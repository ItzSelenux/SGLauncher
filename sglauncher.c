#include "sglauncher.h"
int main(int argc, char *argv[]) 
{
	const char* env_sgcsd = getenv("SGCSD");
	home_dir = getenv("HOME");
	nocsd = (env_sgcsd != NULL) ? atoi(env_sgcsd) == 0 : 0;

	for (int i = 1; i < argc; i++)
	{
		sgcfg = strcmp(argv[i], "--cfg") == 0;
	}

	readconf();
	gtk_init(&argc, &argv);

	create_window();
}