#ifndef APP_H
#define APP_H
#include "models.h"
int ensureDirectories(void); void initializeDefaults(void); int adminLogin(void); void mainMenu(void);
int saveStudent(const Student *s); int findStudentById(const char *id, Student *out); int studentIdExists(const char *id); int rollExists(const char *roll); void generateStudentId(char out[]);
int saveSubject(const Subject *s); int findSubject(const char *code, Subject *out);
int saveMark(const MarkRecord *m); int upsertResult(const SemesterResult *r); int findResult(const char *id,int sem,SemesterResult *out);
float percentage(float a,float b); void getGrade(float p,char out[]); float weakThreshold(void);
int buildRanking(int sem, RankingRecord out[], int max); int getRank(const char *id,int sem,int *total);
void studentMenu(void); void subjectMenu(void); void marksMenu(void); void calculateResult(void); void weakAnalysis(void); void rankingMenu(void); void analyticsMenu(void); void compareSemesters(void); void generateReport(void); void dashboard(void); void settingsMenu(void);
#endif
