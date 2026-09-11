#include <stdio.h>
#include <stdlib.h>

void printLookupData(const int SPECIALITY_ID[], const char *SPECIALITY_NAME[], const float BASE_FEE[],
                    const int CONSALTAION_TIME[], const int DALIY_PATIENT_CAP[],
                    const int WARD_ID[], const char *WARD_NAME[], const float BED_RATE[],
                    const int BED_CAPACITY[]);
void printBedOccupancy(const int bedOccupancy[4][20], const int BED_CAPACITY[], const char *WARD_NAME[]);

int main() {
    const int SPECIALITY_ID[] = {1, 2, 3, 4};
    const char *SPECIALITY_NAME[] = {"General Practice(OPD)", "Paediatrics", "Cardiology", "Neurology"};
    const float BASE_FEE[] = {1500.00, 2500.00, 4500.00, 5000.00};
    const int CONSALTAION_TIME[] = {15, 20, 30, 30};
    const int DALIY_PATIENT_CAP[] = {30, 20, 12, 10};

    int bedOccupancy[4][20] = {0};
    const int WARD_ID[] = {1, 2, 3, 4};
    const char *WARD_NAME[] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU(Intensive Care Unit)"};
    const float BED_RATE[] = {3000.00, 6000.00, 12000.00, 25000.00};
    const int BED_CAPACITY[] = {20, 10, 10, 5};

    printLookupData(SPECIALITY_ID, SPECIALITY_NAME, BASE_FEE, CONSALTAION_TIME, DALIY_PATIENT_CAP,
                    WARD_ID, WARD_NAME, BED_RATE, BED_CAPACITY);


    printBedOccupancy(bedOccupancy, BED_CAPACITY, WARD_NAME);

    return 0;
}

void printLookupData(const int SPECIALITY_ID[], const char *SPECIALITY_NAME[], const float BASE_FEE[],
                    const int CONSALTAION_TIME[], const int DALIY_PATIENT_CAP[],
                    const int WARD_ID[], const char *WARD_NAME[], const float BED_RATE[],
                    const int BED_CAPACITY[]) {
    int i;
    printf("\nDoctor Specialties Data\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-16s| %-24s| %-30s| %-25s |%s|\n", "Specialty ID", "Specialty Name", "Base Consultation Fee (LKR)",
                                                "Consultation Time(mins)", "Daily Patient Cap");
    printf("------------------------------------------------------------------------------------------------------------------------\n");

    for (i = 0; i < 4; i++) {
        printf("%-16d| %-24s| %-30.2f| %-25d |%-17d|\n", SPECIALITY_ID[i], SPECIALITY_NAME[i], BASE_FEE[i],
                                                        CONSALTAION_TIME[i], DALIY_PATIENT_CAP[i]);
    }
    printf("------------------------------------------------------------------------------------------------------------------------\n");

    printf("\nHospital Wards Data\n");
    printf("-----------------------------------------------------------------------------------------------\n");
    printf("%-16s| %-24s| %-30s| %s|\n", "Ward ID", "Ward Name ", "Daily Bed Rate (LKR / Day)", "Total Bed Capacity");
    printf("-----------------------------------------------------------------------------------------------\n");

    for (i = 0; i < 4; i++) {
        printf("%-16d| %-24s| %-30.2f| %-18d|\n", WARD_ID[i], WARD_NAME[i], BED_RATE[i], BED_CAPACITY[i]);
    }
    printf("-----------------------------------------------------------------------------------------------\n");
}

void printBedOccupancy(const int bedOccupancy[4][20], const int BED_CAPACITY[], const char *WARD_NAME[]){
    int w, b;
    printf("\nInitial Bed Occupancy Status (0 = Available, 1 = Occupied)\n");
    printf("-----------------------------------------------------------------------------------------------\n");
    for (w = 0; w < 4; w++) {
        printf("%-26s: [ ", WARD_NAME[w]);
        for (b = 0; b < BED_CAPACITY[w]; b++) {
            printf("%d ", bedOccupancy[w][b]);
        }
        printf("]\n");
    }
    printf("-----------------------------------------------------------------------------------------------\n");
}
