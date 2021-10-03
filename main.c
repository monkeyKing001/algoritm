#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void	update_data(char **bin_data, char **temp_bucket, unsigned int start, unsigned int end)
{
	unsigned int	i;
	unsigned int	j;

	i = start;
	j = 0;
	while (i <= end)
		bin_data[i++] = temp_bucket[j++];
	printf("updated_data\n");
}

void	merge_by_radix(char **bin_data, unsigned int start, unsigned int middle, unsigned int end, int radix)
{
	unsigned int	left_index = start;
	unsigned int	right_index = middle + 1;	

	if (start == end)
		return ;
	//temp_bucket
	char	**temp_bucket =(char **)malloc(sizeof(char *) * (start - end + 1));
	memset(temp_bucket, 0, sizeof(char *) * (start - end + 1));
	printf("merge_by_radix_exce\n");
	unsigned int	temp_index = 0;
	while (left_index <= middle && right_index <= end)
	{
		if(bin_data[left_index][radix] <= bin_data[right_index][radix])
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

void	merge_sort_by_radix(char **bin_data, unsigned int start, unsigned int end, int radix)
{
	printf("merge_sort_by_radix_exce\n");
	if (start != end)
	{
		unsigned int	next_end = (start + end) / 2;
		unsigned int	next_start = (start + end) / 2 + 1;
		merge_sort_by_radix(bin_data, start, next_end, radix);
		merge_sort_by_radix(bin_data, next_start, end, radix);
	}
	unsigned int	middle;
	middle = (start + end) / 2;
	merge_by_radix(bin_data, start, middle, end, radix);
}

void	radix_sort(char **bin_data, unsigned int size, int radix)
{
	printf("radix sort exce\n");
	while (radix >= 0)
		merge_sort_by_radix(bin_data, 0, size - 1, radix--);
}

char *bin_num(long long dec)
{
	char			*bin_num;
	unsigned int	i;
	unsigned int	bit_max;
	
	i = 0;
	bit_max = 32;
	bin_num = malloc(sizeof(char) * bit_max + 1);
	memset(bin_num, '0', bit_max + 1);
	bin_num[bit_max] = '\0';
	while (dec > 0)
	{
		bin_num[bit_max - i] = (dec % 2) + '0';
		i++;
		dec /= 2;
	}
	return (bin_num);
}

int main(int argc, char **argv)
{
	const unsigned int	max = 5;
	unsigned long 		*file_data;
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
		file_data = (unsigned long *)malloc(sizeof(unsigned long) * max);
		memset(file_data, 0, max);
	}
	/* make bin_data */
	bin_data = (char **)malloc(sizeof(char *) * (max + 1));
	memset(bin_data, 0, max + 1);

	unsigned int i = 0;
	while (i < max)
	{
		fscanf(fp, "%ld",&file_data[i]);
		i++;
	}
	/* close file */
	fclose(fp);

	/* print bin_data */
	i = 0;
	while (i < max)
	{
		printf("%ld\n", file_data[i]);
		bin_data[i] = bin_num(file_data[i]);
		printf("bin : %s\n", bin_data[i]);
		i++;
	}

	/* radix_sort */
	radix_sort(bin_data, max, 31);

	/* print bin_data after sorting */
	i = 0;
	while (i < max)
	{
		printf("bin : %s\n", bin_data[i]);
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
