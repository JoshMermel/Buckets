/*
Buckets.cpp, written by Josh Mermelstein
This program prompts the user for the number of buckets they want, and the sizes of those buckets
it then prompts for the desired quantity of water to be made from those buckets
it does simple tests to ensure all these values are positive and consistent with each other
finally, it outputs a linear combination of the bucket sizes equal to the goal quantity

consider including: a function to check if any two buckets are the same size
		    and ask the user if they wish to replace one of them

		    move the calculation of the gcd of the set of buckets[] outside of get_goal
		    as it is used quite frequently.

Last edit		11/06/11
*/

#include <iostream>
#include <math.h>
using namespace std;

int get_int(string message, int index);
void get_sizes(int bucket[], int array_size);
int gcd(int a, int b);
int get_goal(int bucket[], int array_size);
void calc_coefficients(int buckets[], int array_size, int coefficient[]);
void reduce_coefficients(int coefficient[], int array_size, int bucket[]);
void print(int buckets[], int array_size, int coefficient[], int goal);

int main()
{
	//stage 0, various declarations and the input of user data
	//seeming working perfectly as of 10/28 at 1:01 AM.
	int array_size = get_int("how many buckets would you like to use? ", -1);
	int bucket[array_size];
	int coefficient[array_size];
	get_sizes(bucket, array_size);
	int goal = get_goal(bucket, array_size);
	int collective_gcd = 0;

	//stage 1, we fill coefficient[] with the appropriate values such that
	//the dot product of it and bucket[] is the gcd of the bucket sizes
	calc_coefficients(bucket, array_size, coefficient);

	//stage 2, we scale coefficient[] by goal/gcd of the elements of bucket
	collective_gcd = bucket[0];
	for(int i = 1 ; i < array_size ; i++)  //finds gcd of all bucket sizes
	{
		collective_gcd = gcd(bucket[i], collective_gcd);
	}

	for(int i = 0; i < array_size; i++)
	{
		coefficient[i] *= (goal/collective_gcd);
	}
	//stage 3, we reduce the coefficients
	reduce_coefficients(coefficient, array_size, bucket);
	//stage 4, we print
	print(bucket, array_size, coefficient, goal);

	return 0;
}
/*
name	get_int
passed	a string to display
returns	a user supplyed int
*/
int get_int(string message, int index)
{
	int user_input;
	cout << message;
	if(index >= 0)
		cout << '#' << index << ": ";
	cin >> user_input;
	if(user_input <= 0)
	{
		cout << "please enter a value greater than 0 " << endl;
		user_input = get_int(message, index);
	}
	
	return user_input;
}

/*
name	get_sizes
passed	an array and an int of its sizes
returns	nothing
*/
void get_sizes(int bucket[], int array_size)
{
	for(int i = 0 ; i < array_size ; i++)
	{
		bucket[i] = get_int("how much water can be held in bucket ", i);
	}
}

/*
name	gcd
passed	two ints
returns	their greates common divisor
note	this is just the euclidean algorithem implemeted in a while loop
*/
int gcd(int a, int b)
{
	if(b > a)
		return gcd(b, a);
	
	int c;
	c = a % b;
	if(c == 0)
		return b;
	while(c != 0)
	{
		c = a % b;
		a = b;
		b = c;
	}
	return a;
}

/*
name	get_goal
passed	an array of bucket sizes and an int of the size of that array
returns	the goal quantity
notes	gets it own function to make checking if the goal is possible easier
	checks to see if the goal is a divisor of the gcd of the set of buckets
	and if the goal is between 0 and the sum of the sizes of the buckets
*/
int get_goal(int bucket[], int array_size)
{
	int max_volume = 0;
	int goal = get_int("how much water would you like? ", -1);
	int running_gcd = bucket[0];
	for(int i = 1 ; i < array_size ; i++)  //finds gcd of all bucket sizes
	{
		running_gcd = gcd(bucket[i], running_gcd);
	}

	for(int j = 0 ; j < array_size ; j++) //finds sum of buckets volumes
	{
		max_volume += bucket[j];
	}

	if((goal % running_gcd != 0) || (goal > max_volume) || (goal < 0))
	{
		cout << "The desired value cannot be made from the bucket sizes specified." << endl;
		cout << "Please enter a value that is an integer multiple of " << running_gcd;
		cout << " and is between 0 and " << max_volume << endl;
		return get_goal(bucket, array_size);
	}

	return goal;
}

/*
name	calc_coefficients
passed	an array of bucket sizes
	an int of the size of that array
	and array of equal size to hold the coefficents produced
returns	nothing
notes	not actually the extended euclidean algorithem
*/
void calc_coefficients(int buckets[], int array_size, int coefficient[])
{
	int running_gcd = gcd(buckets[0], buckets[1]);
	int co_gcd = 0;
	int future_gcd = 0;
	for(int i = 0; i < array_size ; i++)
	{
		coefficient[i] = 0;
	}

	while((buckets[0] * coefficient[0]) + (buckets[1] * coefficient[1]) != running_gcd)
	{
		if((buckets[0] * coefficient[0]) + (buckets[1] * coefficient[1]) > running_gcd)
		{
			coefficient[1]--;
		}
		else if((buckets[0] * coefficient[0]) + (buckets[1] * coefficient[1]) < running_gcd)
		{
			coefficient[0]++;
		}
	}

	for(int j = 2 ; j < array_size ; j++)
	{
		coefficient[j] = 0;
		co_gcd = 0;
		future_gcd = gcd(running_gcd, buckets[j]);
		while((buckets[j] * coefficient[j]) + (running_gcd * co_gcd) != future_gcd)
		{	
			if((buckets[j] * coefficient[j]) + (running_gcd * co_gcd) > future_gcd)
			{
				coefficient[j]--;
			}
			else if((buckets[j] * coefficient[j]) + (running_gcd * co_gcd) < future_gcd)
			{
				co_gcd++;
			}
		}
		
		//multiple all previous coefficients by co_gcd
		for(int k = 0 ; k < j ; k++)
		{
			coefficient[k] *= co_gcd;
		}
		co_gcd = 0;
		running_gcd = future_gcd;
	}
	
}
/*
name	reduce coefficients
passed	the matrix of coefficients
returns	nothing
purpose	reduce the coefficients
*/
void reduce_coefficients(int coefficient[], int array_size, int bucket[])
{
	for(int i = 0; i < array_size; i++)
	{
		for(int j = i; j < array_size ; j++)
		{
			if(coefficient[i] > 0 && coefficient[j] < 0)
			{
				//first number too big, second too small
				while(coefficient[i] > bucket[j])
				{
					//note, it is unnecessary to handle the case of first too small, second too big as a result of the way the previous functions do their output.
					coefficient[i] -= bucket[j];
					coefficient[j] += bucket[i]; 
				}				
			}
		}
	}
}

/*
name	print
passed	the two arrays of bucket sizes and coefficients, the sizes of those arrays, and the goal value.
returns	nothing
purpose	print the found solution
*/
void print(int buckets[], int array_size, int coefficient[], int goal)
{
	for(int i = 0; i < array_size - 1; i++)
	{
		cout << '(' << coefficient[i] << ")*" << buckets[i] << " + ";
	}
	cout << '(' << coefficient[array_size - 1] << ")*" << buckets[array_size - 1] << " = " << goal << endl;
}


