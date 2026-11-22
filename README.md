# Student Result & Grade Analyzer
C11 console application for student records, subjects, marks, results, weak-subject analysis, rankings, analytics, semester comparison and text reports.

## Build
`gcc -std=c11 -Wall -Wextra -Wpedantic src/*.c -Iinclude -o student_analyzer`

Run from the project root so `data/` and `reports/` resolve correctly.

Default demonstration login: `admin` / `admin123`. Change this before any real use. Authentication is demonstration-level, not production security.
