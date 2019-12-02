





exec = test_node

sources_list = node.c socket_list.c regular_node_config.c\
			    genesis_node_config.c node_loop.c

objects_dir = objs/

sources_dir = src/

objects_list = $(sources_list:.c=.o)
sources = $(addprefix $(sources_dir), $(sources_list))
objects = $(addprefix $(objects_dir), $(objects_list))

all:
	gcc -c $(sources)
	gcc -o $(exec) $(objects_list)
	mv $(objects_list) $(objects_dir)

clean:
	rm -f $(objects)

fclean:
	rm -f $(objects)
	rm -f $(exec)

re: fclean all
