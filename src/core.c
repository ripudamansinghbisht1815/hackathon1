#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "app.h"
static void flushline(void){int c;while((c=getchar())!='\n'&&c!=EOF){}}
static int line(const char *p,char *b,size_t n){printf("%s",p);if(!fgets(b,n,stdin))return 0;char *q=strchr(b,'\n');if(q)*q=0;else flushline();return b[0]!=0;}
int ensureDirectories(void){mkdir("data",0777);mkdir("reports",0777);mkdir("backup",0777);return 1;}
void initializeDefaults(void){ensureDirectories();FILE*f=fopen(CONFIG_FILE,"rb");if(!f){SystemConfig c={DEFAULT_WEAK_THRESHOLD,MAX_SEMESTERS,MAX_SUBJECTS};f=fopen(CONFIG_FILE,"wb");if(f){fwrite(&c,sizeof c,1,f);fclose(f);}}else fclose(f);f=fopen(GRADE_FILE,"rb");if(!f){GradeConfig g[]={{"A+",90,100},{"A",80,90},{"B+",70,80},{"B",60,70},{"C",50,60},{"D",40,50},{"F",0,40}};f=fopen(GRADE_FILE,"wb");if(f){fwrite(g,sizeof g[0],7,f);fclose(f);}}else fclose(f);f=fopen(ADMIN_FILE,"rb");if(!f){Admin a={"admin","admin123"};f=fopen(ADMIN_FILE,"wb");if(f){fwrite(&a,sizeof a,1,f);fclose(f);}}else fclose(f);}
int adminLogin(void){Admin a;FILE*f=fopen(ADMIN_FILE,"rb");if(!f||fread(&a,sizeof a,1,f)!=1){if(f)fclose(f);return 0;}fclose(f);for(int k=3;k;k--){char u[32],p[64];if(!line("Username: ",u,sizeof u)||!line("Password: ",p,sizeof p))return 0;if(!strcmp(u,a.username)&&!strcmp(p,a.password))return 1;printf("Invalid credentials. Attempts left: %d\n",k-1);}return 0;}
int saveStudent(const Student*s){FILE*f=fopen(STUDENT_FILE,"ab");if(!f)return 0;int ok=fwrite(s,sizeof *s,1,f)==1;fclose(f);return ok;}
int findStudentById(const char*id,Student*out){FILE*f=fopen(STUDENT_FILE,"rb");if(!f)return 0;Student s;while(fread(&s,sizeof s,1,f)==1)if(!strcmp(s.studentId,id)){if(out)*out=s;fclose(f);return 1;}fclose(f);return 0;}
int studentIdExists(const char*id){return findStudentById(id,NULL);}int rollExists(const char*r){FILE*f=fopen(STUDENT_FILE,"rb");if(!f)return 0;Student s;while(fread(&s,sizeof s,1,f)==1)if(!strcmp(s.rollNumber,r)){fclose(f);return 1;}fclose(f);return 0;}
void generateStudentId(char out[]){int hi=1000;FILE*f=fopen(STUDENT_FILE,"rb");Student s;if(f){while(fread(&s,sizeof s,1,f)==1){int n;if(sscanf(s.studentId,"STU%d",&n)==1&&n>hi)hi=n;}fclose(f);}snprintf(out,MAX_ID_LENGTH,"STU%d",hi+1);}
int saveSubject(const Subject*s){FILE*f=fopen(SUBJECT_FILE,"ab");if(!f)return 0;int ok=fwrite(s,sizeof *s,1,f)==1;fclose(f);return ok;}
int findSubject(const char*c,Subject*out){FILE*f=fopen(SUBJECT_FILE,"rb");if(!f)return 0;Subject s;while(fread(&s,sizeof s,1,f)==1)if(!strcmp(s.subjectCode,c)){if(out)*out=s;fclose(f);return 1;}fclose(f);return 0;}
int saveMark(const MarkRecord*m){FILE*f=fopen(MARKS_FILE,"ab");if(!f)return 0;int ok=fwrite(m,sizeof *m,1,f)==1;fclose(f);return ok;}
int upsertResult(const SemesterResult*r){SemesterResult a[MAX_STUDENTS*MAX_SEMESTERS];int n=0,found=0;FILE*f=fopen(RESULT_FILE,"rb");if(f){while(n<(int)(sizeof a/sizeof a[0])&&fread(&a[n],sizeof a[n],1,f)==1)n++;fclose(f);}for(int i=0;i<n;i++)if(!strcmp(a[i].studentId,r->studentId)&&a[i].semester==r->semester){a[i]=*r;found=1;}if(!found)a[n++]=*r;f=fopen(RESULT_FILE,"wb");if(!f)return 0;int ok=fwrite(a,sizeof a[0],n,f)==(size_t)n;fclose(f);return ok;}
int findResult(const char*id,int sem,SemesterResult*out){FILE*f=fopen(RESULT_FILE,"rb");if(!f)return 0;SemesterResult r;while(fread(&r,sizeof r,1,f)==1)if(!strcmp(r.studentId,id)&&r.semester==sem){if(out)*out=r;fclose(f);return 1;}fclose(f);return 0;}
float percentage(float a,float b){return b>0?a/b*100.0f:0;}
void getGrade(float p,char out[]){FILE*f=fopen(GRADE_FILE,"rb");GradeConfig g;if(f){while(fread(&g,sizeof g,1,f)==1)if(p>=g.minimumPercentage&&(p<g.maximumPercentage||(p==100&&g.maximumPercentage==100))){strcpy(out,g.grade);fclose(f);return;}fclose(f);}strcpy(out,"N/A");}
float weakThreshold(void){SystemConfig c;FILE*f=fopen(CONFIG_FILE,"rb");if(f&&fread(&c,sizeof c,1,f)==1){fclose(f);return c.weakSubjectThreshold;}if(f)fclose(f);return DEFAULT_WEAK_THRESHOLD;}
static int cmpRank(const void*a,const void*b){float x=((RankingRecord*)b)->percentage-((RankingRecord*)a)->percentage;return x>0?1:x<0?-1:0;}
int buildRanking(int sem,RankingRecord out[],int max){FILE*f=fopen(RESULT_FILE,"rb");if(!f)return 0;SemesterResult r;int n=0;while(n<max&&fread(&r,sizeof r,1,f)==1)if(r.semester==sem){Student s;if(findStudentById(r.studentId,&s)){strcpy(out[n].studentId,s.studentId);strcpy(out[n].rollNumber,s.rollNumber);strcpy(out[n].studentName,s.name);out[n].percentage=r.percentage;out[n].passed=r.passed;n++;}}fclose(f);qsort(out,n,sizeof out[0],cmpRank);for(int i=0;i<n;i++)out[i].rank=(i&&abs((int)((out[i].percentage-out[i-1].percentage)*1000))<1)?out[i-1].rank:i+1;return n;}
int getRank(const char*id,int sem,int*total){RankingRecord a[MAX_STUDENTS];int n=buildRanking(sem,a,MAX_STUDENTS);if(total)*total=n;for(int i=0;i<n;i++)if(!strcmp(a[i].studentId,id))return a[i].rank;return 0;}
