#ifndef MODELS_H
#define MODELS_H
#include "constants.h"
typedef struct { char studentId[MAX_ID_LENGTH]; char rollNumber[MAX_ROLL_LENGTH]; char name[MAX_NAME_LENGTH]; char branch[MAX_BRANCH_LENGTH]; } Student;
typedef struct { char subjectCode[MAX_SUBJECT_CODE]; char subjectName[MAX_SUBJECT_NAME]; char comparisonGroup[MAX_SUBJECT_CODE]; int semester; float maximumMarks; float passingMarks; } Subject;
typedef struct { char studentId[MAX_ID_LENGTH]; char subjectCode[MAX_SUBJECT_CODE]; int semester; float marksObtained; } MarkRecord;
typedef struct { char studentId[MAX_ID_LENGTH]; int semester; float totalMarksObtained; float totalMaximumMarks; float percentage; char overallGrade[MAX_GRADE_LENGTH]; int passed; } SemesterResult;
typedef struct { char grade[MAX_GRADE_LENGTH]; float minimumPercentage; float maximumPercentage; } GradeConfig;
typedef struct { char username[MAX_USERNAME_LENGTH]; char password[MAX_PASSWORD_LENGTH]; } Admin;
typedef struct { float weakSubjectThreshold; int maximumSemesters; int maximumSubjects; } SystemConfig;
typedef struct { char studentId[MAX_ID_LENGTH]; char rollNumber[MAX_ROLL_LENGTH]; char studentName[MAX_NAME_LENGTH]; float percentage; int rank; int passed; } RankingRecord;
#endif
