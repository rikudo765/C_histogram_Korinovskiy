#include "../headers/custom.h"


void init_struct(DType min, DType max, NType M, struct hist *res){
    // Naive constructor
	res->maxHist = max;
	res->minHist = min;
	res->M = M;
	res->frequency = (NType*)calloc(M, sizeof(NType));
	res->bWidth = (res->maxHist - res->minHist) / M;
}

void set_max(DType m, struct hist *h){
    // Set max in structure
	h->maxHist = m;
}

void set_min(DType m, struct hist *h){
    // Set min in structure
	h->minHist = m;
}

void set_m(NType m, struct hist *h)
{
	h->M = m;
    // Resetting histogram frequency
	free(h->frequency);
	h->frequency = (NType*)calloc(m, sizeof(NType));
	h->bWidth = (h->maxHist - h->minHist) / m;
}

int add_number(DType x, struct hist *h){
    // Adding number to our histogram
    // Check if number in our hist
	if (x < h->minHist || x > h->maxHist){return -1;}
	int index;
	index = (int)((x - h->minHist) / h->bWidth);
	h->frequency[index]++;
	return 0;
}

int add_batch(DType data[], size_t s, struct hist *h){
    // Add sequence of numbers
	for (NType i = 0; i < s; i++){
		int status = add_number(data[i], h);
		if (status == -1){
			return status;
		}
	}
	return 0;
}

int add_batch_from_file(char *file_path, char mode, struct hist *h)
// mode :
    // 't' for text
    // 'b' for binary
{
	if (mode == 'b'){
		DType data[128] = {0};
		size_t size = 128;
		long *fpos = (long*)calloc(1, sizeof(long));
		while (true){
			int status = _batch_helper_bin(file_path, data, size, fpos);
			// If data not fit our hist
			if (status == -1){
				free(fpos);
				return -1;
			}
			else if (status < (int)size){
				add_batch(data, status, h);	
				return 0;
			}
			else{
				add_batch(data, size, h);
			}
		}
	}
	else if (mode == 't'){
		DType data[128] = { 0 };
		size_t size = 128;
		long *fpos = (long*)calloc(1, sizeof(long));
		while (true){
			int status = _batch_helper_text(file_path, data, size, fpos);
            // If data not fit our hist
			if (status == -1){
				free(fpos);
				return -1;
			}
			else if (status < (int)size){
				add_batch(data, status, h);	
				return 0;
			}
			else{
				add_batch(data, size, h);
			}
		}
	}
	else return -1;
}

int _batch_helper_text(char *file_path, DType *data, size_t size, long *fpos){
// Helping function to read from the file
	FILE *input = NULL;
	input = fopen(file_path, "r");
    // Reading from last place of interruption
	fseek(input, *fpos, 0);
    // If  file wasn't opened
	if (input == NULL){
		fclose(input);
		return -1;
	}
	int proc_amount = 0;
    // Reading for each integer
	for (NType i = 0; i < size; i++){
	    // If end of file
		if (feof(input)){
			fclose(input);
			return proc_amount;
		}
		// Read data into double type
		fscanf(input, "%lf", &data[i]);
		proc_amount++;
	}
	*fpos = ftell(input);
	fclose(input);
	return proc_amount;
}

int _batch_helper_bin(char *file_path, DType *data, size_t size, long *fpos){
// Helping function to read from the binary file
	FILE *input = NULL;
	input = fopen(file_path, "rb");
    // Reading from last place of interruption 
	fseek(input, *fpos, 0);
    // If  file wasn't opened
	if (input == NULL){
		fclose(input);
		return -1;
	}
	int proc_amount = 0;
	for (NType j = 0; j < size; j++){
		char str[20];
		char flag = 0;
		for (NType i = 0; i < 10; i++){
		    // Binary data must have separated int nums
			char temp;
			// Read byte-by-byte
			fread(&temp, sizeof(char), 1, input);
			// If end of reading
			if (feof(input)){
				flag = 1;
				break;
			}
			// It temp == [\n,\t, ]
			if (temp == 10 || temp == 32 || temp == 9){
				break;
			}
			str[i] = temp;
		}
		data[j] = (DType)atof(str);
		proc_amount++;
		// Check for end of file
		if (flag){
			break;
		}
	}
	// Change last file position
	*fpos = ftell(input);
	fclose(input);
	return proc_amount;
}

int _print_frequency(struct hist *h){
    // Prints frequencies in hist
	for (NType i = 0; i < h->M; i++){
		printf("-.-%d-.-", h->frequency[i]);
	}
	printf("\n");
	for (NType i = 0; i < h->M; i++){
		printf("-%.2f-", h->minHist + i * h->bWidth);
	}
	return 0;
}

NType num(struct hist *h){
    // Calculates amount of elements in hist
	NType res = 0;
	for (NType i = 0; i < h->M; i++)
	{
		res += h->frequency[i];
	}
	return res;
}

NType numHist(struct hist *h, NType i){
    // Calculates amount of elements in current "bucket"
	if (i < 0 || i > h->M - 1)
	{
		return -1;
	}
	return h->frequency[i];
}

DType mean(struct hist *h){
    // Calculates men value of hist
	DType res = 0;
	DType tmp = h->minHist + h->bWidth / 2;
	for (NType i = 0; i < h->M; i++)
	{
		res += h->frequency[i] * tmp;
		tmp += h->bWidth;
	}
	return (DType)res/(DType)(num(h));
}

DType median(struct hist *h){
    // Calculates median value of hist
	NType amount = num(h);
	DType res = h->minHist;
	DType mid = (DType)amount/2;
	NType cur = 0;
	for (NType i = 0; i < h->M; i++){
		cur += h->frequency[i];
		// If pass half-way -> median value in previous "bucket"
		if (cur >= mid){
			res += h->bWidth / 2;
			return res;
		}
		else{
			res += h->bWidth;
		}
	}
	return res;
}

DType dev(struct hist *h){
    // Calculates deviance of hist
	return pow(var(h), 0.5);
}

DType var(struct hist *h){
    //Calculates variance of histogram
	DType res = 0;
	DType m = mean(h);
	NType n = num(h); 
	DType tmp = h->minHist + h->bWidth/2;
	for (NType i = 0; i < h->M; i++)
	{
		res += pow((tmp - m), 2) * h->frequency[i];
		tmp += h->bWidth;
	}
	return res/(n);
}