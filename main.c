#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -----------------------------------------
//FUNCTION PROTOTYPES DECLARATION
//------------------------------------------
void printLookupData(const int SPECIALITY_ID[], const char *SPECIALITY_NAME[], const float BASE_FEE[],
                     const int CONSALTAION_TIME[], const int DALIY_PATIENT_CAP[],
                     const int WARD_ID[], const char *WARD_NAME[], const float BED_RATE[],
                     const int BED_CAPACITY[]);
float calculateWaitingTime(int tempSpec,int specialtyQueueCount[],const int CONSALTAION_TIME[]);
void printBedOccupancy(const int bedOccupancy[4][20], const int BED_CAPACITY[], const char *WARD_NAME[]);
float calculatEmergencySurcharge(int emergencyLevel[],const float BASE_FEE[],int tempSpec,int patientCount);
float wardDailyRate(int wardId[],int patientCount);
float calculateTotalWardCost(int daysAdmitted[],float wardDailyRate,int patientCount);
float calculateGrossTotalBill(const float BASE_FEE[],float calculatedemergencySurcharge,float calclulatedTotalWardCost,int tempSpec);
float calculatedFinalAmountPayable(int patientAge[],float calclulatedGrossTotalBill,int patientCount);
void sortAndDisplay(char patientName[][50],int patientAge[],int emergencyLevel[],int specialtyID[],
                    int checkAdmitted[],int wardId[],int daysAdmitted[],int patientCount);
void generatePerformanceReport(char patientName[][50], int emergencyLevel[], int checkAdmitted[], int wardId[], int daysAdmitted[], int patientCount,
                               float totalRevenue, float totalDiscounts, char highestPayingPatient[], float maxFinalAmount,const int BED_CAPACITY[]);
int main()
{
    //--------------------------------------------------
    // CONSTANT LOOKUP TABLES & SYSTEM CONFIGURATION DATA
    //---------------------------------------------------
    const int SPECIALITY_ID[] = {1, 2, 3, 4};
    const char *SPECIALITY_NAME[] = {"General Practice(OPD)", "Paediatrics", "Cardiology", "Neurology"};
    const float BASE_FEE[] = {1500.00, 2500.00, 4500.00, 5000.00};
    const int CONSALTAION_TIME[] = {15, 20, 30, 30};
    const int DALIY_PATIENT_CAP[] = {30, 20, 12, 10};

    const int WARD_ID[] = {1, 2, 3, 4};
    const char *WARD_NAME[] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU(Intensive Care Unit)"};
    const float BED_RATE[] = {3000.00, 6000.00, 12000.00, 25000.00};
    const int BED_CAPACITY[] = {20, 10, 10, 5};

    // 2D Array Matrix for Bed Occupancy Tracking (4 Wards, Max 20 Beds each; 0=Available, 1=Occupied)
    int bedOccupancy[4][20] = {0};

    //--------------------------------------------------
    // DYNAMIC PATIENT STORAGE ARRAYS & SYSTEM COUNTERS
    //--------------------------------------------------
    char patientName[100][50];
    float estimateTime =0.0;
    int patientAge[100];
    int emergencyLevel[100];
    int specialtyID[100];
    int checkAdmitted[100];
    int wardId[100];
    int tempSpec = 0;
    int daysAdmitted[100];
    float maxFinalAmount = 0.0;
    char highestPayingPatient[100] = "";
    float totalRevenue = 0.0;
    float totalDiscounts = 0.0;

    int specialtyQueueCount[4] = {0, 0, 0, 0};// Real-time patient count per specialty
    int patientCount = 0;// Total registered patient counter
    int choice = 0;// User menu selection flag

    //--------------------------------------------------
    // MAIN MENU DRIVEN CONTROL LOOP
    //--------------------------------------------------
    do
    {
        printf("\n==========================================\n");
        printf("   SMART HOSPITAL MANAGEMENT SYSTEM       \n");
        printf("==========================================\n");
        printf("1. Display Lookup Data & Bed Occupancy\n");
        printf("2. Register New Patient (Full Intake Process) & Print the Bill\n");
        printf("3. Billing & Waiting Time Calculations\n");
        printf("4. Performance Reports & Analytics\n");
        printf("5. Exit Program\n");
        printf("Your choice : ");
        scanf("%d",&choice);

        switch (choice)
        {
        case 1:
            // Display static lookup tables and current ward bed matrix status
            printLookupData(SPECIALITY_ID, SPECIALITY_NAME, BASE_FEE, CONSALTAION_TIME, DALIY_PATIENT_CAP,
                            WARD_ID, WARD_NAME, BED_RATE, BED_CAPACITY);
            printBedOccupancy(bedOccupancy, BED_CAPACITY, WARD_NAME);
            break;

        case 2:
        {
            //Patient Personal Details
            printf("\n--- Patient Details (Patient ID: PAT-%d) ---\n", 1001 + patientCount);
            printf("Patient Name : ");
            scanf(" %[^\n]s", patientName[patientCount]);

            printf("Patient Age (years) : ");
            scanf("%d", &patientAge[patientCount]);

            do
            {
                printf("Emergency / Triage Level (1 = Normal, 2 = Urgent, 3 = Critical) : ");
                scanf("%d",&emergencyLevel[patientCount]);
                if (emergencyLevel[patientCount] < 1 || emergencyLevel[patientCount] > 3)
                {
                    printf("Invalid Input! Enter 1, 2, or 3.\n");
                }
            }
            while (emergencyLevel[patientCount] < 1 || emergencyLevel[patientCount] > 3);

            //Specialty Selection
            printf("\n--- Specialty Selection ---\n");
            do
            {
                printf("Specialty ID (1 to 4) : ");
                scanf("%d", &tempSpec);
                if (tempSpec < 1 || tempSpec > 4)
                {
                    printf("Invalid Specialty ID! Enter between 1 and 4.\n");
                }
            }
            while (tempSpec < 1 || tempSpec > 4);

            specialtyID[patientCount] = tempSpec;
            estimateTime = calculateWaitingTime(tempSpec,specialtyQueueCount,CONSALTAION_TIME);
            specialtyQueueCount[tempSpec - 1]++;
            printf("Selected Specialty: %s | Base Fee: LKR %.2f\n",SPECIALITY_NAME[tempSpec - 1],BASE_FEE[tempSpec - 1]);

            //Ward Admission & Bed Allocation
            printf("\n--- Ward Admission Details ---\n");
            do
            {
                printf("Is Admitted to Ward? (1 = Yes, 0 = No) : ");
                scanf("%d",&checkAdmitted[patientCount]);
                if (checkAdmitted[patientCount] != 0 && checkAdmitted[patientCount] != 1)
                {
                    printf("Invalid Input! Enter 1 for Yes or 0 for No.\n");
                }
            }
            while (checkAdmitted[patientCount] != 0 && checkAdmitted[patientCount] != 1);

            //validation for specialty and ward selections(Invalid IDs)
            if (checkAdmitted[patientCount] == 1)
            {
                do
                {
                    printf("Input Ward ID (1 to 4) : ");
                    scanf("%d", &wardId[patientCount]);
                    if (wardId[patientCount] < 1 || wardId[patientCount] > 4)
                    {
                        printf("Invalid Ward ID! Enter between 1 and 4.\n");
                    }
                }
                while (wardId[patientCount] < 1 || wardId[patientCount] > 4);

                do
                {
                    printf("Input Days Admitted : ");
                    scanf("%d", &daysAdmitted[patientCount]);
                    if (daysAdmitted[patientCount] <= 0)
                    {
                        printf("Invalid Input! Days admitted must be greater than 0.\n");
                    }
                }
                while (daysAdmitted[patientCount] <= 0);


                int selectedWard = wardId[patientCount] - 1;
                int bedAllocated = 0;

                //bed allocation logic and occupancy matrix updates
                for (int b = 0; b < BED_CAPACITY[selectedWard]; b++)
                {
                    if (bedOccupancy[selectedWard][b] == 0)
                    {
                        bedOccupancy[selectedWard][b] = 1;
                        bedAllocated = 1;
                        printf("Bed Allocated Successfully! Ward: %s | Bed No: %d\n",
                               WARD_NAME[selectedWard], b + 1);
                        break;
                    }
                }

                if (!bedAllocated)
                {
                    printf("Warning: No available beds in %s!\n", WARD_NAME[selectedWard]);
                }

            }
            else
            {
                wardId[patientCount] = 0;
                daysAdmitted[patientCount] = 0;
                printf("Patient registered as OPD (No Bed Allocated).\n");
            }

            printf("\nPatient Registration Successful!!\n");

            //financial calculation & invoice generation
            float calculatedemergencySurcharge = calculatEmergencySurcharge(emergencyLevel,BASE_FEE,tempSpec,patientCount);
            float daliyRate = wardDailyRate(wardId,patientCount);
            float calclulatedTotalWardCost = calculateTotalWardCost(daysAdmitted,daliyRate,patientCount);
            float calclulatedGrossTotalBill = calculateGrossTotalBill(BASE_FEE,calculatedemergencySurcharge,calclulatedTotalWardCost,tempSpec);
            float discount = calculatedFinalAmountPayable(patientAge,calclulatedGrossTotalBill,patientCount);
            float finalAmount = calclulatedGrossTotalBill -discount;

            //find total revenue & total discount
            totalRevenue += finalAmount;
            totalDiscounts += discount;
            //find max payable customer & amount
            float tempmax = finalAmount;
            if(maxFinalAmount < tempmax)
            {
                maxFinalAmount = tempmax;
                strcpy(highestPayingPatient,patientName[patientCount]);
            }
            else if(maxFinalAmount == tempmax)
            {
                strcat(highestPayingPatient,",");
                strcat(highestPayingPatient,patientName[patientCount]);
            }
            //print final amount and Bill

            printf("\n==================================================\n");
            printf("                PATIENT INVOICE / RECEIPT          \n");
            printf("==================================================\n");
            printf("Patient ID               : PAT-%d\n", 1001 + patientCount);
            printf("Patient Name             : %s\n", patientName[patientCount]);
            printf("Age                      : %d\n", patientAge[patientCount]);
            printf("Emergency Level          : %d\n", emergencyLevel[patientCount]);
            printf("Specialty Selected       : %s\n", SPECIALITY_NAME[tempSpec - 1]);
            printf("Base Consultation Fee    : LKR %.2f\n", BASE_FEE[tempSpec - 1]);
            printf("Emergency Surcharge      : LKR %.2f\n", calculatedemergencySurcharge);
            printf("Total Ward Cost          : LKR %.2f\n", calclulatedTotalWardCost);
            printf("--------------------------------------------------\n");
            printf("Gross Total Bill         : LKR %.2f\n", calclulatedGrossTotalBill);
            printf("Age Discount Amount      : LKR %-.2f\n", discount);
            printf("--------------------------------------------------\n");
            printf("FINAL AMOUNT PAYABLE     : LKR %.2f\n", finalAmount);
            printf("Estimated Waiting Time   : %.2f mins\n",estimateTime);
            printf("==================================================\n");

            patientCount++;
            break;
        }

        case 3:
        {
            if(patientCount==0)
            {
                printf("\nNo patients registered yet to display!\n");

            }
            else
            {
                // View sorted priority patient queue
                sortAndDisplay(patientName,patientAge,emergencyLevel,specialtyID,
                               checkAdmitted,wardId,daysAdmitted,patientCount);
            }
            break;
        }
        case 4:
        {
            generatePerformanceReport(patientName,emergencyLevel,checkAdmitted,wardId,daysAdmitted,patientCount,
                                      totalRevenue,totalDiscounts,highestPayingPatient,maxFinalAmount,BED_CAPACITY);
            break;
        }

        case 5:
        {
            printf("\nThank for get service from us !!\n");
            break;
        }

        default:
        {
            printf("\nInvalid Choice! Enter a number between 1-4.\n");
            break;
        }
        }

    }
    while (choice != 5);

    return 0;
}
// Displays Doctor Specialty details and Hospital Ward Lookup tables
void printLookupData(const int SPECIALITY_ID[], const char *SPECIALITY_NAME[], const float BASE_FEE[],
                     const int CONSALTAION_TIME[], const int DALIY_PATIENT_CAP[],
                     const int WARD_ID[], const char *WARD_NAME[], const float BED_RATE[],
                     const int BED_CAPACITY[])
{
    int i;
    printf("\nDoctor Specialties Data\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-16s| %-24s| %-30s| %-25s |%s|\n", "Specialty ID", "Specialty Name", "Base Consultation Fee (LKR)",
           "Consultation Time(mins)", "Daily Patient Cap");
    printf("------------------------------------------------------------------------------------------------------------------------\n");

    for (i = 0; i < 4; i++)
    {
        printf("%-16d| %-24s| %-30.2f| %-25d |%-17d|\n", SPECIALITY_ID[i], SPECIALITY_NAME[i], BASE_FEE[i],
               CONSALTAION_TIME[i], DALIY_PATIENT_CAP[i]);
    }
    printf("------------------------------------------------------------------------------------------------------------------------\n");

    printf("\nHospital Wards Data\n");
    printf("-----------------------------------------------------------------------------------------------\n");
    printf("%-16s| %-24s| %-30s| %s|\n", "Ward ID", "Ward Name ", "Daily Bed Rate (LKR / Day)", "Total Bed Capacity");
    printf("-----------------------------------------------------------------------------------------------\n");

    for (i = 0; i < 4; i++)
    {
        printf("%-16d| %-24s| %-30.2f| %-18d|\n", WARD_ID[i], WARD_NAME[i], BED_RATE[i], BED_CAPACITY[i]);
    }
    printf("-----------------------------------------------------------------------------------------------\n");
}
// Displays 2D matrix representing real-time occupancy status per ward
void printBedOccupancy(const int bedOccupancy[4][20], const int BED_CAPACITY[], const char *WARD_NAME[])
{
    int w, b;
    printf("\nInitial Bed Occupancy Status (0 = Available, 1 = Occupied)\n");
    printf("-----------------------------------------------------------------------------------------------\n");
    for (w = 0; w < 4; w++)
    {
        printf("%-26s: [ ", WARD_NAME[w]);
        for (b = 0; b < BED_CAPACITY[w]; b++)
        {
            printf("%d ", bedOccupancy[w][b]);
        }
        printf("]\n");
    }
    printf("-----------------------------------------------------------------------------------------------\n");
}
float calculateWaitingTime(int tempSpec,int specialtyQueueCount[],const int CONSALTAION_TIME[])
{
    float estimatedWait = specialtyQueueCount[tempSpec-1] * CONSALTAION_TIME[tempSpec-1];
    return estimatedWait;
}
// Calculates percentage-based surcharge based on Triage Emergency Level
float calculatEmergencySurcharge(int emergencyLevel[],const float BASE_FEE[],int tempSpec,int patientCount)
{

    float totlSurcharge = 0.0;
    float surchargePercentage = 0.0;
    switch(emergencyLevel[patientCount])
    {
    case 1 :
        surchargePercentage = 0.0;
        break;
    case 2 :
        surchargePercentage = 0.2;
        break;
    default :
        surchargePercentage = 0.5;
        break;
    }
    totlSurcharge = BASE_FEE[tempSpec-1] * surchargePercentage;
    return totlSurcharge;
}
// Lookup Daily Bed Rate for Ward ID
float wardDailyRate(int wardId[],int patientCount)
{
    float wardRate = 0.0;
    switch(wardId[patientCount])
    {
    case 1:
        wardRate = 3000.0; //General Ward Rate
        break;
    case 2:
        wardRate = 6000.0; //Paediatric WardaRate
        break;
    case 3:
        wardRate = 12000.0; //Surgical Ward Rate
        break;
    default:
        wardRate = 25000.0; //ICU Rate
        break;
    }
    return wardRate;
}
// Total Ward Stay Cost calculation
float calculateTotalWardCost(int daysAdmitted[],float wardDailyRate,int patientCount)
{
    float totalWardCost = 0.0;

    totalWardCost = daysAdmitted[patientCount] * wardDailyRate;
    return totalWardCost;
}
// Gross Total Bill Calculation
float calculateGrossTotalBill(const float BASE_FEE[],float calculatedemergencySurcharge,float calclulatedTotalWardCost,int tempSpec)
{
    float totalBill = BASE_FEE[tempSpec-1] + calculatedemergencySurcharge + calclulatedTotalWardCost;
    return totalBill;
}
//age based Discount calculation
float calculatedFinalAmountPayable(int patientAge[],float calclulatedGrossTotalBill,int patientCount)
{
    float discount = 0.0;

    if(patientAge[patientCount] > 0 && (patientAge[patientCount] < 5 || patientAge[patientCount] > 65))
    {
        discount = calclulatedGrossTotalBill * 0.15;//15% discount
    }
    else
    {
        discount = 0.0;
    }

    return discount;
}
// Bubble Sort Algorithm: Sorts patients in descending order based on Emergency Triage Level (3 -> 2 -> 1)
void sortAndDisplay(char patientName[][50],int patientAge[],int emergencyLevel[],int specialtyID[],
                    int checkAdmitted[],int wardId[],int daysAdmitted[],int patientCount)
{
    int i,j;
    char tempname[50];
    int tempAge, tempEmerg, tempSpec, tempAdmit, tempWard, tempDays;

    for(i=0; i<patientCount-1; i++)
    {
        for(j=0; j<patientCount-i-1; j++)
        {
            if(emergencyLevel[j]<emergencyLevel[j+1])
            {
                //swap emergency Level
                tempEmerg = emergencyLevel[j];
                emergencyLevel[j] = emergencyLevel[j+1];
                emergencyLevel[j+1] = tempEmerg;

                // Swap Patient Name String
                strcpy(tempname,patientName[j]);
                strcpy(patientName[j],patientName[j+1]);
                strcpy(patientName[j+1],tempname);

                // Swap Patient Age
                tempAge = patientAge[j];
                patientAge[j] = patientAge[j+1];
                patientAge[j+1] = tempAge;

                // Swap Specialty ID
                tempSpec = specialtyID[j];
                specialtyID[j] = specialtyID[j+1];
                specialtyID[j+1] = tempSpec;

                // Swap Admission Status
                tempAdmit = checkAdmitted[j];
                checkAdmitted[j] = checkAdmitted[j+1];
                checkAdmitted[j+1] = tempAdmit;

                // Swap Ward ID
                tempWard = wardId[j];
                wardId[j] = wardId[j+1];
                wardId[j+1] = tempWard;

                // Swap Days Admitted
                tempDays = daysAdmitted[j];
                daysAdmitted[j] = daysAdmitted[j+1];
                daysAdmitted[j+1] = tempDays;

            }
        }
    }
    // Display Triage Priority Queue Output
    printf("\n---------------------------------------------------------------------------------------\n");
    printf("                  REGISTERED PATIENTS LIST (PRIORITY ORDER)                              \n");
    printf("-----------------------------------------------------------------------------------------\n");
    printf("%-5s | %-20s | %-5s | %-18s | %-12s | %-10s\n",
           "No.", "Patient Name", "Age", "Urgency Level", "Specialty ID", "Admitted");
    printf("----------------------------------------------------------------------------------------\n");

    for(i=0; i<patientCount; i++)
    {
        char urgencyText[20];
        if(emergencyLevel[i] == 3)
        {
            strcpy(urgencyText,"Level 3 (Critical)");
        }
        else if(emergencyLevel[i] == 2)
        {
            strcpy(urgencyText,"Level 2 (Urgent))");
        }
        else
        {
            strcpy(urgencyText,"Level 1 (Normal)");
        }
        printf("%-5d | %-20s | %-5d | %-18s | %-12d | %-10s\n",
               i + 1, patientName[i], patientAge[i], urgencyText, specialtyID[i],
               (checkAdmitted[i] == 1 ? "Yes" : "No"));//use ternary operator to check addmitted
    }
    printf("----------------------------------------------------------------------------------------\n");
}
//Performance Reports & Analytics
void generatePerformanceReport(char patientName[][50], int emergencyLevel[], int checkAdmitted[], int wardId[], int daysAdmitted[], int patientCount,
                               float totalRevenue, float totalDiscounts, char highestPayingPatient[], float maxFinalAmount,const int BED_CAPACITY[])
{
    int i;
    int countLevel1 = 0,countLevel2 = 0,countLevel3 = 0;
    int wardOccupancy[4] = {0,0,0,0};

    if(patientCount == 0)
    {
        printf("\nNo patient data available to generate reports!!\n");
    }
    for(i=0; i<patientCount; i++)
    {
        if(emergencyLevel[i]== 1) countLevel1++;
        else if(emergencyLevel[i]==2) countLevel2++;
        else countLevel3++;

        if(checkAdmitted[i] == 1 && wardId[i] >= 0 && wardId[i] <= 4)
        {
            wardOccupancy[wardId[i]-1]++;
        }
    }
    printf("\n=========================================================\n");
    printf("            PERFORMANCE REPORTS & ANALYTICS              \n");
    printf("=========================================================\n");

    printf("\n[1] PATIENT REGISTRATION SUMMARY\n");
    printf("     Total Patients Registered : %d\n", patientCount);
    printf("      * Level 1 (Normal)       : %d\n", countLevel1);
    printf("      * Level 2 (Urgent)       : %d\n", countLevel2);
    printf("      * Level 3 (Critical)     : %d\n", countLevel3);

    printf("\n[2] FINANCIAL SUMMARY\n");
    printf("      -Total Revenue Earned    : LKR %.2f\n", totalRevenue);
    printf("      -Total Discounts Granted : LKR %.2f\n", totalDiscounts);

    printf("\n[3] BED OCCUPANCY PERCENTAGE PER WARD\n");
    for (int i = 0; i < 4; i++)
    {
        float tempWardOccupancy = wardOccupancy[i];
        float percentage = (tempWardOccupancy / BED_CAPACITY[i]) * 100.0;
        printf("Ward %d: %d/%d Beds Occupied (%.2f%%)\n",i + 1, wardOccupancy[i], BED_CAPACITY[i],percentage);
    }
    printf("\n[4] HIGHEST-PAYING PATIENT\n");
    if(maxFinalAmount > 0)
    {
        printf("       -Patient Name : %s\n",highestPayingPatient);
        printf("       -Total Bill   : LKR %.2f\n",maxFinalAmount);
    }
    else
    {
        printf("No billing data recorded yet.\n");
    }
    printf("=========================================================\n");
    return;
}
