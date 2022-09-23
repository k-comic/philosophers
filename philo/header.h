/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leroy <leroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 08:57:04 by leroy             #+#    #+#             */
/*   Updated: 2022/06/10 00:45:01 by leroy            ###   ########.fr       */
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

typedef struct s_rules
{
	int				number_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				at_least;
	struct timeval	time_start;
	pthread_mutex_t	*m_printf;
	pthread_mutex_t	*m_change;
	int				dead_or_enough;
	int				fed_philos;
}	t_rules;

typedef struct s_philo
{
	int				id;
	pthread_mutex_t	*cheap_fork;
	pthread_mutex_t	*expen_fork;
	struct timeval	last_meal;
	int				meals;
	t_rules			*rules;
}	t_philo;

long	time_from(struct timeval from);
void	ft_print(t_philo *philo, char *message);
int		ft_atoi(const char *str);
int		init_ptr(t_philo **philos, pthread_mutex_t **mutexes,
			pthread_t **threads, t_rules *rules);
int		init_mutexes(t_rules rules,
			t_philo *philos, pthread_mutex_t *mutexes, pthread_t *threads);
void	init_philos(t_rules *rules, t_philo *philos, pthread_mutex_t *mutexes);
int		init_threads(t_rules rules,
			t_philo *philos, pthread_mutex_t *mutexes, pthread_t *threads);
void	*philo_life(void *data);

#endif