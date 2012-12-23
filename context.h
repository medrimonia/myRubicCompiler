
typedef struct context * context_pointer

struct context{
	context_pointer father_context;
	dictionnary_pointer global_variables;
	dictionnary_pointer local_variables;
	dictionnary_pointer instance_variables;
	dictionnary_pointer constants;
	dictionnary_pointer types;
	dictionnary_pointer classes;
	dictionnary_pointer functions;
}
