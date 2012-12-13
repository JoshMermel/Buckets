/*
 *
 * Last edit        12/12/12
 *
 */

#include <iostream>
#include <math.h>
using namespace std;

struct GCDstruct {
    int d, X, Y;
};

int get_int(string message);
int get_uniq_int(int *buckets, int fullness);
void get_sizes(int* bucket, int num_buckets);
int gcd(int a, int b);
GCDstruct eGCD(int a, int b);
int get_goal(int bucket[], int num_buckets);
void calc_coefficients(int *buckets, int num_buckets, int *coefficient);
void reduce_coefficients(int coefficient[], int num_buckets, int bucket[]);
void print(int buckets[], int num_buckets, int coefficient[], int goal);

int main()
{
    int num_buckets = get_int("how many buckets would you like to use? ");
    int* bucket = new int[num_buckets];
    int* coefficient = new int[num_buckets];

    get_sizes(bucket, num_buckets);
    int goal = get_goal(bucket, num_buckets);

    // fill the coefficients array such that the dot product of buckets and
    // coefficients is goal
    int collective_gcd = 0;
    calc_coefficients(bucket, num_buckets, coefficient);

    //finds gcd of all bucket sizes
    collective_gcd = bucket[0];
    for (int i = 1 ; i < num_buckets ; i++)
        collective_gcd = gcd(bucket[i], collective_gcd);

    // scale coefficient[] by goal/gcd of the elements of bucket
    int scalar = goal/collective_gcd;
    for (int i = 0; i < num_buckets; i++)
        coefficient[i] *= scalar;

    reduce_coefficients(coefficient, num_buckets, bucket);
    reduce_coefficients(coefficient, num_buckets, bucket);
    print(bucket, num_buckets, coefficient, goal);

    delete [] bucket;
    delete [] coefficient;

    return 0;
}

// prints a message and gets a positive integer from the user
int get_int(string message) {
    int user_input;
    cout << message;
    cin >> user_input;
    if(user_input <= 0) {
        cout << "please enter a value greater than 0 " << endl;
        user_input = get_int(message);
    }
    return user_input;
}

// puts a user specified int into the buckets array
// won't accept ints which are already in the array
int get_uniq_int(int *bucket, int fullness){
    int user_input;
    cin >> user_input;
    for (int i = 0; i < fullness; i++) {
        if(user_input == bucket[i]) {
            cout << "that size was already used, please enter a unique size\n"
                << "How much water can be held in bucket " << fullness << ": ";
            return get_uniq_int(bucket, fullness);
        }
    }
    return user_input;
}

// fills the array of bucket sizes using user input from get_uniq_int
void get_sizes(int* bucket, int num_buckets) {
    int user_input;
    for (int i = 0 ; i < num_buckets ; i++) {
        cout << "How much water can be held in bucket " << i << ": ";
        bucket[i] = get_uniq_int(bucket, i);
    }
}

// calculates the gcd of two ints using the euclidean algorithm
int gcd(int a, int b) {
    if(a % b == 0)  {return b;}
    return gcd(b, a % b);
}

// does the extended euclidean algorithm on a and b
GCDstruct eGCD(int a, int b) {
    if( a % b == 0) {
        GCDstruct ret = {b, 0, 1};
        return ret;
    }
    GCDstruct ret;
    int q = a / b;
    int r = a % b;
    ret = eGCD(b, r);
    int temp = ret.X;
    ret.X = ret.Y;
    ret.Y = temp - (ret.Y * q);
    return ret;
}

int get_goal(int bucket[], int num_buckets) {
    int max_volume = 0;
    int goal = get_int("how much water would you like? ");
    int running_gcd = bucket[0];
    //finds gcd of all bucket sizes
    for(int i = 1 ; i < num_buckets ; i++)  
        running_gcd = gcd(bucket[i], running_gcd);
    //finds sum of buckets volumes
    for(int j = 0 ; j < num_buckets ; j++)
        max_volume += bucket[j];

    if((goal % running_gcd != 0) || (goal > max_volume) || (goal < 0)) {
        cout << "The desired value cannot be made"
            "from the bucket sizes specified.\n";
        cout << "Please enter a value that is an integer multiple of "
            << running_gcd;
        cout << " and is between 0 and " << max_volume << endl;
        return get_goal(bucket, num_buckets);
    }

    return goal;
}

void calc_coefficients(int *buckets, int num_buckets, int *coefficient) {
    int running_gcd = gcd(buckets[0], buckets[1]);
    int co_gcd = 0;
    int future_gcd = 0;
    for(int i = 0; i < num_buckets ; i++)
    {
        coefficient[i] = 0;
    }

    GCDstruct calc = eGCD(buckets[0], buckets[1]);
    coefficient[0] = calc.X;
    coefficient[1] = calc.Y;

    for(int j = 2 ; j < num_buckets ; j++)
    {
        coefficient[j] = 0;
        co_gcd = 0;
        calc = eGCD(buckets[j], running_gcd);
        coefficient[j] = calc.X;
        for(int k = 0 ; k < j ; k++)
            coefficient[k] *= calc.Y;
        co_gcd = 0;
        running_gcd = gcd(running_gcd, buckets[j]);
    }

}

void reduce_coefficients(int coefficient[], int num_buckets, int bucket[]) {
    for(int i = 0; i < num_buckets; i++)
        for(int j = i; j < num_buckets ; j++){
            // case first number too small, second too big
            if(coefficient[i] < 0 && coefficient[j] > 0)
                while(coefficient[i] < bucket[j])
                {
                    coefficient[i] += bucket[j];
                    coefficient[j] -= bucket[i]; 
                }
            // case first number too big, second too small
            if(coefficient[i] > 0 && coefficient[j] < 0)
                while(coefficient[i] > bucket[j])
                {
                    coefficient[i] -= bucket[j];
                    coefficient[j] += bucket[i]; 
                }
        }
}

void print(int buckets[], int num_buckets, int coefficient[], int goal) {
    for(int i = 0; i < num_buckets - 1; i++)
        cout << '(' << coefficient[i] << ")*" << buckets[i] << " + ";
    cout << '(' << coefficient[num_buckets - 1] << ")*" 
        << buckets[num_buckets - 1] << " = " << goal << endl;
}
