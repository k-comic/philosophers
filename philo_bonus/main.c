/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leroy <leroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 08:57:21 by leroy             #+#    #+#             */
/*   Updated: 2022/06/10 02:52:35 by leroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	check_args(int argc, char **argv, t_rules *rules)
{
	if (argc < 5 || 6 < argc)
	{
		write(2, "Error: Wrong arguments.\n", 24);
		return (-1);
	}
	if (ft_atoi(argv[1]) < 1 || ft_atoi(argv[2]) < 0 || ft_atoi(argv[3]) < 0
		|| ft_atoi(argv[4]) < 0 || (argc == 6 && ft_atoi(argv[5]) < 0))
	{
		write(2, "Error: Wrong format.\n", 21);
		return (-1);
	}
	rules->number_of_philos = ft_atoi(argv[1]);
	rules->time_to_die = ft_atoi(argv[2]);
	rules->time_to_eat = ft_atoi(argv[3]);
	rules->time_to_sleep = ft_atoi(argv[4]);
	rules->at_least = -1;
	rules->fed_philos = 0;
	if (argc == 6)
	{
		if (ft_atoi(argv[5]) == 0)
			return (0);
		rules->at_least = ft_atoi(argv[5]);
	}
	return (1);
}

static void	exit_sim(t_rules *rules, t_philo *philos)
{
	int	i;
	int	ret;

	i = 0;
	while (i < rules->number_of_philos)
	{
		waitpid(-1, &ret, 0);
		if (ret != 0)
		{
			i = 0;
			while (i < rules->number_of_philos)
			{
				kill(philos[i].proc_id, 15);
				i++;
			}
			break ;
		}
		i++;
	}
	sem_close(rules->forks);
	sem_close(rules->print);
	sem_close(rules->change);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/change");
}

static int	start_sim(t_rules *rules, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < rules->number_of_philos)
	{
		philos[i].proc_id = fork();
		if (philos[i].proc_id < 0)
			return (-1);
		if (philos[i].proc_id == 0)
			philo_life(&(philos[i]));
		usleep(100);
		i++;
	}
	exit_sim(rules, philos);
	return (0);
}

int	main(int argc, char **argv)
{
	t_rules			rules;
	t_philo			*philos;

	if (check_args(argc, argv, &rules) < 1)
		return (-1);
	if (init_ptr(&philos, &rules))
		return (-1);
	if (init_sems(&rules))
		return (-1);
	init_philos(&rules, philos);
	if (start_sim(&rules, philos))
		printf("There was an error creating the threads");
	free(philos);
	return (0);
}
