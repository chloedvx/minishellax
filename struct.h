#ifndef STRUCT_H
# define STRUCT_H

typedef struct s_data {
	char	**envp;
	char	**var;
	char	*oldpwd;
	char	*pwd;
	int	pipe0[2];
	int	pipe1[2];
	int	*pipes[2];
} t_data;

typedef struct s_token
{
	token_type	token;
	token_cmd	cmd;
	char 		*data;
	size_t		data_size;
	struct s_token	*next;
} t_token;

typedef struct s_heads
{
	t_token	*line;
	t_token	*cmd;
	t_token	*infile;
	t_token	*outfile;
}t_heads;

#endif
