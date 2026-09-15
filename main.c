#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//function prototype
void printLookupData(const int SPECIALITY_ID[], const char *SPECIALITY_NAME[], const float BASE_FEE[],
                    const int CONSALTAION_TIME[], const int DALIY_PATIENT_CAP[],
                    const int WARD_ID[], const char *WARD_NAME[], const float BED_RATE[],
                    const int BED_CAPACITY[]);
void calculateWaitingTime(const char *SPECIALITY_NAME[],int specialtyQueueCount[],const int CONSALTAION_TIME[]);
void printBedOccupancy(const int bedOccupancy[4][20], const int BED_CAPACITY[], const char *WARD_NAME[]);
float calculatEmergencySurcharge(int emergencyLevel[],const float BASE_FEE[],int tempSpec,int patientCount);
float wardDailyRate(int wardId[],int patientCount);
float calculateTotalWardCost(int daysAdmitted[],float wardDailyRate,int patientCount);
float calculateGrossTotalBill(const float BASE_FEE[],float calculatedemergencySurcharge,float calclulatedTotalWardCost,int tempSpec);
float calculatedFinalAmountPayable(int patientAge[],float calclulatedGrossTotalBill,int patientCount);

int main() {
    const int SPECIALITY_ID[] = {1, 2, 3, 4};
    const char *SPECIALITY_NAME[] = {"General Practice(OPD)", "Paediatrics", "Cardiology", "Neurology"};
    const float BASE_FEE[] = {1500.00, 2500.00, 4500.00, 5000.00};
    const int CONSALTAION_TIME[] = {15, 20, 30, 30};
    const int DALIY_PATIENT_CAP[] = {30, 20, 12, 10};

    const int WARD_ID[] = {1, 2, 3, 4};
    const char *WARD_NAME[] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU(Intensive Care Unit)"};
    const float BED_RATE[] = {3000.00, 6000.00, 12000.00, 25000.00};
    const int BED_CAPACITY[] = {20, 10, 10, 5};

    int bedOccupancy[4][20] = {0};

    char patientName[100][50];
    int patientAge[100];
    int emergencyLevel[100];
    int specialtyID[100];
    int checkAdmitted[100];
    int wardId[100];
    int tempSpec = 0;
    int daysAdmitted[100];

    int specialtyQueueCount[4] = {0, 0, 0, 0};
    int patientCount = 0;
    int choice = 0;

    //driven menu
    do {
        printf("\n==========================================\n");
        printf("   SMART HOSPITAL MANAGEMENT SYSTEM       \n");
        printf("==========================================\n");
        printf("1. Display Lookup Data & Bed Occupancy\n");
        printf("2. Register New Patient (Full Intake Process)\n");
        printf("3. Estimated Waiting Time Report\n");
        printf("4. Exit Program\n");
        printf("Your choice : ");
        scanf("%d",&choice);

        switch (choice){
            case 1:
                printLookupData(SPECIALITY_ID, SPECIALITY_NAME, BASE_FEE, CONSALTAION_TIME, DALIY_PATIENT_CAP,
                                WARD_ID, WARD_NAME, BED_RATE, BED_CAPACITY);
                printBedOccupancy(bedOccupancy, BED_CAPACITY, WARD_NAME);
                break;

            case 2: {
                //Patient Personal Details
                printf("\n--- Patient Details (Patient ID: PAT-%d) ---\n", 1001 + patientCount);
                printf("Patient Name : ");
                scanf(" %[^\n]s", patientName[patientCount]);

                printf("Patient Age (years) : ");
                scanf("%d", &patientAge[patientCount]);

                do {
                    printf("Emergency / Triage Level (1 = Normal, 2 = Urgent, 3 = Critical) : ");
                    scanf("%d",&emergencyLevel[patientCount]);
                    if (emergencyLevel[patientCount] < 1 || emergencyLevel[patientCount] > 3) {
                        printf("Invalid Input! Enter 1, 2, or 3.\n");
                    }
                } while (emergencyLevel[patientCount] < 1 || emergencyLevel[patientCount] > 3);

                //Specialty Selection
                printf("\n--- Specialty Selection ---\n");
                do {
                    printf("Specialty ID (1 to 4) : ");
                    scanf("%d", &tempSpec);
                    if (tempSpec < 1 || tempSpec > 4) {
                        printf("Invalid Specialty ID! Enter between 1 and 4.\n");
                    }
                } while (tempSpec < 1 || tempSpec > 4);

                specialtyID[patientCount] = tempSpec;
                specialtyQueueCount[tempSpec - 1]++;
                printf("Selected Specialty: %s | Base Fee: LKR %.2f\n",
                       SPECIALITY_NAME[tempSpec - 1],
                       BASE_FEE[tempSpec - 1]);

                //Ward Admission & Bed Allocation
                printf("\n--- Ward Admission Details ---\n");
                do {
                    printf("Is Admitted to Ward? (1 = Yes, 0 = No) : ");
                    scanf("%d",&checkAdmitted[patientCount]);
                    if (checkAdmitted[patientCount] != 0 && checkAdmitted[patientCount] != 1) {
                        printf("Invalid Input! Enter 1 for Yes or 0 for No.\n");
                    }
                } while (checkAdmitted[patientCount] != 0 && checkAdmitted[patientCount] != 1);
                //validation for specialty and ward selections(Invalid IDs)
                if (checkAdmitted[patientCount] == 1) {
                    do {
                        printf("Input Ward ID (1 to 4) : ");
                        scanf("%d", &wardId[patientCount]);
                        if (wardId[patientCount] < 1 || wardId[patientCount] > 4) {
                            printf("Invalid Ward ID! Enter between 1 and 4.\n");
                        }
                    } while (wardId[patientCount] < 1 || wardId[patientCount] > 4);

                    do {
                        printf("Input Days Admitted : ");
                        scanf("%d", &daysAdmitted[patientCount]);
                        if (daysAdmitted[patientCount] <= 0) {
                            printf("Invalid Input! Days admitted must be greater than 0.\n");
                        }
                    } while (daysAdmitted[patientCount] <= 0);


                    int selectedWard = wardId[patientCount] - 1;
                    int bedAllocated = 0;

                     //bed allocation logic and occupancy matrix updates
                    for (int b = 0; b < BED_CAPACITY[selectedWard]; b++) {
                        if (bedOccupancy[selectedWard][b] == 0) {
                            bedOccupancy[selectedWard][b] = 1;
                            bedAllocated = 1;
                            printf("Bed Allocated Successfully! Ward: %s | Bed No: %d\n",
                                   WARD_NAME[selectedWard], b + 1);
                            break;
                        }
                    }

                    if (!bedAllocated) {
                        printf("Warning: No available beds in %s!\n", WARD_NAME[selectedWard]);
                    }

                } else {
                    wardId[patientCount] = 0;
                    daysAdmitted[patientCount] = 0;
                    printf("Patient registered as OPD (No Bed Allocated).\n");
                }

                printf("\nPatient Registration Successful!!\n");

                float calculatedemergencySurcharge = calculatEmergencySurcharge(emergencyLevel,BASE_FEE,tempSpec,patientCount);
                float daliyRate = wardDailyRate(wardId,patientCount);
                float calclulatedTotalWardCost = calculateTotalWardCost(daysAdmitted,daliyRate,patientCount);
                float calclulatedGrossTotalBill = calculateGrossTotalBill(BASE_FEE,calculatedemergencySurcharge,calclulatedTotalWardCost,tempSpec);
                calculatedFinalAmountPayable(patientAge,calclulatedGrossTotalBill,patientCount);

                patientCount++;
                break;
            }

            case 3: {
                printf("\n--- Estimated Waiting Time Report ---\n");
                calculateWaitingTime(SPECIALITY_NAME,specialtyQueueCount,CONSALTAION_TIME);
                }
                break;

            case 4:{
                printf("\nThank for get service from us !!\n");
                break;
            }

            default:{
                printf("\nInvalid Choice! Enter a number between 1-4.\n");
                break;
              }
        }

    } while (choice != 4);

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

void printBedOccupancy(const int bedOccupancy[4][20], const int BED_CAPACITY[], const char *WARD_NAME[]) {
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
void calculateWaitingTime(const char *SPECIALITY_NAME[],int specialtyQueueCount[],const int CONSALTAION_TIME[]){
        int s;
        for (s = 0; s < 4; s++) {
            int estimatedWait = specialtyQueueCount[s] * CONSALTAION_TIME[s];
            printf("Specialty: %-25s | Queue: %-3d | Est. Wait: %d mins\n",SPECIALITY_NAME[s], specialtyQueueCount[s], estimatedWait);
        }
}
float calculatEmergencySurcharge(int emergencyLevel[],const float BASE_FEE[],int tempSpec,int patientCount){

    float totlSurcharge = 0.0;
    float surchargePercentage = 0.0;
    switch(emergencyLevel[patientCount]){
        case 1 :surchargePercentage = 0.0;
            break;
        case 2 :surchargePercentage = 0.2;
            break;
        default :surchargePercentage = 0.5;
        break;
    }
    totlSurcharge = BASE_FEE[tempSpec-1] * surchargePercentage;
    return totlSurcharge;
}
// Lookup Daily Bed Rate for Ward ID
float wardDailyRate(int wardId[],int patientCount){
    float wardRate = 0.0;
    switch(wardId[patientCount]){
        case 1: wardRate = 3000.0; //General Ward Rate
            break;
        case 2: wardRate = 6000.0; //Paediatric WardaRate
            break;
        case 3: wardRate = 12000.0; //Surgical Ward Rate
            break;
        default: wardRate = 25000.0; //ICU Rate
        break;
    }
    return wardRate;
}
// Total Ward Stay Cost calculation
float calculateTotalWardCost(int daysAdmitted[],float wardDailyRate,int patientCount){
    float totalWardCost = 0.0;

    totalWardCost = daysAdmitted[patientCount] * wardDailyRate;
    return totalWardCost;
}
// Gross Total Bill Calculation
float calculateGrossTotalBill(const float BASE_FEE[],float calculatedemergencySurcharge,float calclulatedTotalWardCost,int tempSpec){
    float totalBill = BASE_FEE[tempSpec-1] + calculatedemergencySurcharge + calclulatedTotalWardCost;
    return totalBill;
}
//age based Discount calculation && finalamount
float calculatedFinalAmountPayable(int patientAge[],float calclulatedGrossTotalBill,int patientCount){
    float discount = 0.0;

    if(patientAge[patientCount] > 0 && (patientAge[patientCount] < 5 || patientAge[patientCount] > 65)){
        discount = calclulatedGrossTotalBill * 0.15;//15% discount
    }
    else{
        discount = 0.0;
    }
    float finalAmount = calclulatedGrossTotalBill - discount;

    return finalAmount;
}
