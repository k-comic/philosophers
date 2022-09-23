/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leroy <leroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 08:57:04 by leroy             #+#    #+#             */
/*   Updated: 2022/06/10 02:37:33 by leroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>
# include <semaphore.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>

typedef struct s_rules
{
	int				number_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				at_least;
	struct timeval	time_start;
	sem_t			*forks;
	sem_t			*change;
	sem_t			*print;
	int				fed_philos;
}	t_rules;

typedef struct s_philo
{
	int				id;
	int				proc_id;
	pthread_t		death_check;
	int				dead;
	struct timeval	last_meal;
	int				meals;
	t_rules			*rules;
}	t_philo;

long	time_from(struct timeval from);
void	ft_print(t_philo *philo, char *message);
int		ft_atoi(const char *str);
int		init_ptr(t_philo **philos, t_rules *rules);
int		init_sems(t_rules *rules);
void	init_philos(t_rules *rules, t_philo *philos);
void	*philo_life(void *data);

#endif