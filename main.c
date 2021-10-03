#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int		bit_cmp_radix(char **bin_data, int start_ind, int radix, unsigned int ind_1, unsigned int ind_2)
{
	int	i = 0;
	while (i < radix)
	{
		if (bin_data[ind_1][start_ind + i] != bin_data[ind_2][start_ind + i])
			return ((bin_data[ind_1][start_ind + i] - bin_data[ind_2][start_ind + i]));
		i++;
	}
	i--;
	return (bin_data[ind_1][start_ind + i] - bin_data[ind_1][start_ind + i]);
}

void	update_data(char **bin_data, char **temp_bucket, unsigned int start, unsigned int end)
{
	unsigned int	i;
	unsigned int	j;

	i = start;
	j = 0;
	while (i <= end){
		bin_data[i] = temp_bucket[j];
		i++;
		j++;
	}
}

void	merge_by_radix(char **bin_data, unsigned int start, unsigned int middle, unsigned int end, int start_bit_index, int radix)
{
	unsigned int	left_index = start;
	unsigned int	right_index = middle + 1;	

	if (start == end)
		return ;
	//temp_bucket
	char	**temp_bucket =(char **)malloc(sizeof(char *) * (end - start + 1));
	memset(temp_bucket, 0, sizeof(char *) * (end - start + 1));
	unsigned int	temp_index = 0;
	while (left_index <= middle && right_index <= end)
	{
		if(bit_cmp_radix(bin_data, start_bit_index, radix, left_index, right_index) <= 0)
		{
			temp_bucket[temp_index] = bin_data[left_index];
			temp_index++;
			left_index++;
		}
		else
		{
			temp_bucket[temp_index] = bin_data[right_index];
			temp_index++;
			right_index++;
		}
	}
	while (left_index <= middle)
	{
		temp_bucket[temp_index] = bin_data[left_index];
		temp_index++;
		left_index++;
	}
	while (right_index <= end)
	{
		temp_bucket[temp_index] = bin_data[right_index];
		temp_index++;
		right_index++;
	}
	update_data(bin_data, temp_bucket, start, end);
	free(temp_bucket);
}

void	merge_sort_by_radix(char **bin_data, unsigned int start, unsigned int end, int start_bit_index, int radix)
{
	if (start != end)
	{
		unsigned int	next_end = (start + end) / 2;
		unsigned int	next_start = (start + end) / 2 + 1;
		merge_sort_by_radix(bin_data, start, next_end, start_bit_index ,radix);
		merge_sort_by_radix(bin_data, next_start, end, start_bit_index, radix);
	}
	unsigned int	middle;
	middle = (start + end) / 2;
	merge_by_radix(bin_data, start, middle, end, start_bit_index, radix);
}

void	radix_sort(char **bin_data, unsigned int size, int radix)
{
	int	offset = radix;
	while (64 - offset >= 0){
		merge_sort_by_radix(bin_data, 0, size - 1, 64 - offset, radix);
		offset += radix;
	}
}

char *bin_num(unsigned long long dec)
{
	char	*bin_num;
	int		bit_max;
	
	bit_max = 64;
	bin_num = malloc(sizeof(char) * (bit_max + 1));
	memset(bin_num, '0', sizeof(char) * (bit_max + 1));
	bin_num[bit_max] = 0;
	while (--bit_max >= 0)
	{
		bin_num[bit_max] = (dec % 2) + '0';
		dec /= 2;
	}
	return (bin_num);
}

int main(int argc, char **argv)
{
	const unsigned int	max = 7000000;
	unsigned long long 	*file_data;
	char				*file_data_str;
	char				**bin_data;
	FILE				*fp;
	
	if (argc != 2)
	{
		printf("error : unvalid argument count\n");
		return (0);
	}

	/* file open */
	fp = fopen(argv[1], "r");
	if (fp != NULL)
	{
		file_data = (unsigned long long *)malloc(sizeof(unsigned long long) * max);
		memset(file_data, 0, sizeof(unsigned long long) * max);
		file_data_str = (char *)malloc(sizeof(char) * (50 + 1));
		memset(file_data_str, '0', sizeof(char) * (50 + 1));
	}
	/* make bin_data */
	bin_data = (char **)malloc(sizeof(char *) * (max + 1));
	memset(bin_data, 0, sizeof(char *) * (max + 1));

	unsigned int i = 0;
	while (i < max)
	{
		fscanf(fp, "%lld", &file_data[i]);
		i++;
	}
	/* close file */
	fclose(fp);

	/* print bin_data */
	i = 0;
	while (i < max)
	{
//		printf("%lld\n", file_data[i]);
		bin_data[i] = bin_num(file_data[i]);
//		printf("bin : %s\n", bin_data[i]);
		i++;
	}

	/* radix_sort */
	int	radix = 4;
	radix_sort(bin_data, max, radix);

	printf("\n\t\t======= sorting completed!! =========\n\n");

	/* print bin_data after sorting */
	i = 0;
	while (i < max)
	{
		printf("bin[%d] : %s\n", i ,bin_data[i]);
		i++;
	}
	/* free bin_data */
	i = 0;
	while (i < max)
	{
		free(bin_data[i]);
		i++;
	}
	free(bin_data);
    return (0);
}  
