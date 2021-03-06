#include "../include/nand_validator.h"

unsigned int	get_size_nand(FILE **log, char *nand_filename)
{
	struct stat	nand_stat;

	if (lstat(nand_filename, &nand_stat) < 0)
	{
		if (stat(nand_filename, &nand_stat) < 0)
		{
			fprintf(stderr, "error stat: %s: %s\n",  strerror(errno), nand_filename);
			write_log_time(log);
			fprintf(*log, "error stat: %s: %s\n",  strerror(errno), nand_filename);
			exit(EXIT_FAILURE);
		}
	}
	return(nand_stat.st_size);
}

int				check_nand_valid(FILE **log, unsigned int *nand, int *nand_type)
{
	int				i = 0;
	unsigned int	size[] = {	988807168, 1979711488, 1000341504, 1300234240,	/*2DS nand*/
								988807168, 1000341504,							/*Old 3ds_nand*/
								1979711488, 1300234240, 1954545664				/*N3ds nand*/
	};

	while  (i < SIZE_LEN)
	{
		if (*nand == size[i])
		{
			write_log_time(log);
			fprintf(*log, "Size nand = %d octets\n", *nand);
			if ((size[i] == 988807168 || size[i] == 1979711488) && *nand_type == 0)
			{
				write_log_time(log);
				fprintf(*log, "Toshiba nand detected\n");
				printf("Toshiba nand detected\n");
			}
			else if ((size[i] == 1000341504 || size[i] == 1300234240 || size[i] == 1954545664)
			&& *nand_type == 0)
			{
				write_log_time(log);
				fprintf(*log, "Samsung nand detected\n");
				printf("Samsung nand detected\n");
			}
			*nand_type = 1;
			return (EXIT_SUCCESS);
		}
		++i;
	}
	return (EXIT_FAILURE);
}

void	check_size_nand(FILE **log, unsigned int *size_nand1, unsigned int *size_nand2, char *nand1, char *nand2)
{
	int	nand_type = 0;

	write_log_time(log);
	fprintf(*log, "Checking size %s ...\n", nand1);
	if (*size_nand1 != *size_nand2)
	{
		printf(RED"Size %s and %s are not same\n"END, nand1, nand2);
		write_log_time(log);
		fprintf(*log, "Size %s and %s are not same\n", nand1, nand2);
		fclose(*log);
		exit(EXIT_FAILURE);
	}
	if (check_nand_valid(log, size_nand1, &nand_type) == EXIT_FAILURE)
	{
		printf(RED"Size of %s is bad\n"END, nand1);
		write_log_time(log);
		fprintf(*log, "Size nand = %d octets\n", *size_nand1);
		write_log_time(log);
		fprintf(*log, "Size of %s is bad\n", nand1);
		fclose(*log);
		exit (EXIT_FAILURE);
	}
	write_log_time(log);
	fprintf(*log, "Size of %s [Ok]\n", nand1);
	write_log_time(log);
	fprintf(*log, "Checking size %s ...\n", nand2);
	/*check if nand2 have a good size*/
	if (check_nand_valid(log, size_nand2, &nand_type) == EXIT_FAILURE)
	{
		printf(RED"Size of %s is bad\n"END, nand2);
		write_log_time(log);
		fprintf(*log, "Size nand = %d octets\n", *size_nand2);
		write_log_time(log);
		fprintf(*log, "Size of %s is bad\n", nand2);
		fclose(*log);
		exit (EXIT_FAILURE);
	}
	write_log_time(log);
	fprintf(*log, "Size of %s [Ok]\n", nand2);
}
