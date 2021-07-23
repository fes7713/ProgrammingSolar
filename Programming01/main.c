#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define TRY 8
#define LOOP 250
#define DAY 2000
//font 28

int maxArrayPos(double arr[], int size);
void printVarArray2(int rows, int cols, int MaxCols, char arr[][MaxCols]);
int Limit(int nNum, int limit);
long int sumArray(long int arr[], int size);
long int returnBoundary(long int lower, long int upper, long int Boundary1, long int Boundary2, long int Boundary3);
void fieldWriting(int day, int arrData[], long int MaxNum);
void saveOutput(FILE * cfPtr, int rows, int cols, char arr[][cols]);

typedef struct{
    int baseRate;
    int recovery;
    int death;
}   Rate;

typedef struct{
    int vertical;
    int horizontal;
}   Shift;

typedef struct{
    long int Num;
    long int Day;
} Max;

typedef struct{
    double Value;
    double Next;
    long int Cut;
    long int Cut1;
    long int Cut2;
    long int Cut5;
} Axis;

int fieldHeight = 20;
int fieldWidth = 50;
char field[22][70] = {};
double printAmpY = 0;
Shift AxisShift = {2, 4};
Axis nY;

int main(int argc, char **argv)
{
    float Staying = 0;
    float RadiusReducer = 0;

    long int infected = 200;
    long int prevInfected;
    long int arrInfected[DAY] = {};
    long int arrNewInfected[DAY] = {};
    long int arrIsolated[DAY] = {};

    int arrDayDField[fieldWidth];
    float radius = 12.5;
    int nRecovered = 0;
    int nDeath = 0;
    int nIsolated = 0;
    long int Population = 10000000;

    Rate infectRate = {20, 10, 9};
    Rate isolateRate = {10, 20, 6};
    Max infectMax = {0, 0};
    Max increaseMax = {0, 0};
    time_t t = time(NULL);
    FILE * cfPtr ;

    char date[64];
    char fname[64] = "output";

    strftime(date, sizeof(date), "%Y_%m%d_%H%M%S", localtime(&t));
    strcat(fname, date);
    strcat(fname, ".txt");
    
    if((cfPtr = fopen(fname, "a"))== NULL)
    {
        printf("file open error");
        return -1;
    }
    
    srand(time(NULL));
    
    for(int i = 0; i < DAY; i++)
    {
        //getchar();
        //Staying = -atan(0.05 * i + 1.558) + 2;
        RadiusReducer = 1 / (0.005 * i + 1);
        
        int effPopInRad = (infected - sumArray(arrIsolated, i)) * radius * (Population - nRecovered - nDeath - infected) / Population * Staying * RadiusReducer;
        //int effPopInRad = Limit((int)(radius * (infected - sumArray(arrIsolated, i))), Population - nRecovered - infected - nDeath - sumArray(arrIsolated, i));
        
//Probability simulation
//Infection
        for(int j = 0; j < effPopInRad; j++)
        {
            if(rand() % 100 < infectRate.baseRate)
            {
                infected++;
                arrNewInfected[i]++;
            }
        }
//Recovery
        for(int j = 0; j < infected - sumArray(arrIsolated, i); j++)
        {
            if(rand() % 1000 < infectRate.recovery)
            {
                infected--;
                nRecovered++;
            }
        }
//Death
        for(int j = 0; j < infected - sumArray(arrIsolated, i); j++)
        {
            if(rand() % 10000 < infectRate.death)
            {
                infected--;
                nDeath++;
            }
        }
//Isolated
        for(int j = 0; j <= i - 10; j++)
        {
            for(int k = 0; k < arrNewInfected[j]; k++)
            {
                if(rand() % 100 < isolateRate.baseRate)
                {
                    arrNewInfected[j]--;
                    arrIsolated[j]++;
                }
            }
        }
//Isolated Death
        for(int j = 0; j <= i; j++)
        {
            for(int k = 0; k < arrIsolated[j]; k++)
            {
                if(rand() % 10000 < isolateRate.death)
                {
                    arrIsolated[j]--;
                    infected--;
                    nDeath++;
                }
            }
        }
//Isolated Recovered
        for(int j = 0; j <= i; j++)
        {
            for(int k = 0; k < arrIsolated[j]; k++)
            {
                if(rand() % 1000 < isolateRate.recovery)
                {
                    arrIsolated[j]--;
                    infected--;
                    nRecovered++;
                }
            }
        }

        if(infected <= 0)
            break;

//Preparation
        if(printAmpY < infected / (double)fieldHeight){
            printAmpY = infected / (double)fieldHeight;
            infectMax.Num = infected;
            infectMax.Day = i;
        }
        else{
            if(infected < sumArray(arrIsolated, i))
                infected = sumArray(arrIsolated, i);
        }
        if(increaseMax.Num < infected - prevInfected)
        {
            increaseMax.Num = infected - prevInfected;
            increaseMax.Day = i;
        }
            
        double printAmpX = fieldWidth / (double)i;
        arrInfected[i] = infected;
        
        fieldWriting(i, arrInfected, infectMax.Num);
        
        system("cls");
         
        fprintf(cfPtr, "Day %3d\n", i);
        //fprintf(cfPtr, "printAmpY %7.2lf\n", printAmpY);
        fprintf(cfPtr, "Infected: %d(Day Ratio %.3lf %+d) \nRecovery: %d(%.3f%%) Death A: %ld(A: %.3f%%)\nIsolated: %d(%.3f%%)\n", 
            infected, (double)infected / prevInfected, infected - prevInfected, 
            nRecovered, nRecovered / (float)Population * 100,  nDeath, nDeath / (float)(infected + nDeath + nRecovered) * 100, sumArray(arrIsolated, i), sumArray(arrIsolated, i) / (float)infected * 100);
        fprintf(cfPtr, "Max infection : %d (Day %d) Max increase (Day %d): %d\n", infectMax.Num, infectMax.Day, increaseMax.Day,  increaseMax.Num);
        prevInfected = infected;

        saveOutput(cfPtr, fieldHeight + AxisShift.vertical, fieldWidth + 20, field);
        printVarArray2(fieldHeight + AxisShift.vertical, fieldWidth + AxisShift.horizontal + 1, fieldWidth + 20, field);
        int l = 0;
    }
    fclose(cfPtr);
    return 0;
}

int maxArrayPos(double arr[], int size)
{
    double nMax = arr[0];
    int nMaxPos = 0;
    for(int i = 1; i < size; i++)
    {
        if(nMax < arr[i])
        {
            nMax = arr[i];
            nMaxPos = i;
        }
    }
    return nMaxPos;
}

void printVarArray2(int rows, int cols, int MaxCols, char arr[][MaxCols])
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            printf("%c", arr[i][j]);
        }
        printf("\n");
    }
}

int Limit(int nNum, int limit)
{
    if(nNum > limit)
    {
        return limit;
    }
    return nNum;
}

long int sumArray(long int arr[], int size)
{
    long int sum = 0;
    for(int i = 0; i <= size; i++)
    {
        sum += arr[i];
    }
    
    return sum;
}

long int returnBoundary(long int lower, long int upper, long int Boundary1, long int Boundary2, long int Boundary3)
{
    if(lower < Boundary1 && Boundary1 < upper)
        return Boundary1;
    if(lower < Boundary2 && Boundary2 < upper)
        return Boundary2;
    if(lower < Boundary3 && Boundary3 < upper)
        return Boundary3;
    return 0;
}

void fieldWriting(int day, int arrData[], long int MaxNum)
{
    memset(field, 0, (fieldHeight + AxisShift.vertical) * (fieldWidth + 20));
    int arrDayField[DAY];
    for(int j = 0; j < (day < fieldWidth ? day + 1 : fieldWidth); j++)
    {
        if(day < fieldWidth)
            arrDayField[j] = j;
        else
            arrDayField[j] = (int)(round(day / (double)(fieldWidth - 1) * j)); 
    }
    AxisShift.horizontal = (int)(log10(MaxNum)) + 1;
        
//Grid writing
    for(int j = 0; j < fieldHeight; j++)
       {
        nY.Value = printAmpY * j;
        nY.Next = printAmpY * (j + 1);
        nY.Cut1 = pow(10, floor(log10(nY.Value)) + 1);
        nY.Cut2 = pow(10, floor(log10(nY.Value / 2)) + 1) * 2;
        nY.Cut5 = pow(10, floor(log10(nY.Value / 5)) + 1) * 5;
        nY.Cut = returnBoundary(nY.Value, nY.Next, nY.Cut1, nY.Cut2, nY.Cut5);
        if(nY.Cut > 0)
        {
//y Axis scale writing
            for(int k = 0; k < fieldWidth + AxisShift.horizontal + 1; k++)
            {
                if(k == 0)
                {
                    char yAxisValue[AxisShift.horizontal];
                    snprintf(yAxisValue, AxisShift.horizontal, "%-4ld", nY.Cut);
                    strncpy(&field[fieldHeight - j][0], yAxisValue, AxisShift.horizontal);
                }
                if(k > AxisShift.horizontal)
                    field[fieldHeight - j][k] = '_';
            }
        }
        field[fieldHeight - 1 - j][AxisShift.horizontal] = '|';
    }

//x Axis scale writing
        for(int j = 0; j < fieldWidth; j++)
        {
            field[fieldHeight - 1][j + AxisShift.horizontal + 1] = '_';
//            double nXCut = fmodf(arrDayDField[j], 50) * 50 + 50;
//            if(i < 50 ? j % 5 == 0 : arrDayDField[j] < nXCut && nXCut < arrDayDField[j + 1])
//            {
//                char xAxisValue[3];
//                snprintf(xAxisValue, 1, "%3d", i);
//                strncpy(&field[fieldHeight - 1][j], xAxisValue, 3);
//            }
        }

//Field writing
    for(int j = 0; j < (day < fieldWidth ? day + 1 : fieldWidth); j++)
            for(int k = fieldHeight - 1; k > fieldHeight - 1 - (int)(arrData[arrDayField[j]] / printAmpY) ; k--)
                field[k][j + AxisShift.horizontal + 1] = '*';
}

void saveOutput(FILE * cfPtr, int rows, int cols, char arr[][cols])
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            fprintf(cfPtr, "%c", arr[i][j]);
        }
        fprintf(cfPtr, "\n");
    }
}