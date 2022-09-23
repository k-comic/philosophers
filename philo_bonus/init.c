/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leroy <leroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 03:10:27 by leroy             #+#    #+#             */
/*   Updated: 2022/06/10 01:34:56 by leroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	init_ptr(t_philo **philos, t_rules *rules)
{
	*philos = (t_philo *)malloc(sizeof(t_philo) * rules->number_of_philos);
	if (!philos)
	{
		printf("Error: malloc philos failed.\n");
		return (-1);
	}
	return (0);
}

int	init_sems(t_rules *rules)
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/change");
	rules->forks = sem_open("/forks", O_CREAT, S_IRWXU,
			rules->number_of_philos);
	rules->print = sem_open("/print", O_CREAT, S_IRWXU, 1);
	rules->change = sem_open("/change", O_CREAT, S_IRWXU, 1);
	if (rules->forks == SEM_FAILED
		|| rules->print == SEM_FAILED || rules->change == SEM_FAILED)
		return (-1);
	return (0);
}

void	init_philos(t_rules *rules, t_philo *philos)
{
	int	i;

	i = 0;
	gettimeofday(&(rules->time_start), NULL);
	while (i < rules->number_of_philos)
	{	
		philos[i].id = i;
		philos[i].rules = rules;
		philos[i].meals = 0;
		philos[i].last_meal = rules->time_start;
		philos[i].dead = 0;
		i++;
	}
}
