#include "sglauncher.h"
int main(int argc, char *argv[]) 
{
	const char* env_sgcsd = getenv("SGCSD");
	home_dir = getenv("HOME");
	nocsd = (env_sgcsd != NULL) ? atoi(env_sgcsd) == 0 : 0;
	fcsd = (env_sgcsd != NULL) ? 1 : 0;

	for (int i = 1; i < argc; i++)
	{
		sgcfg = strcmp(argv[i], "--cfg") == 0;
		if (strcmp(argv[i], "--nocsd") == 0)
		{
			nocsd = 1;
			fcsd = 1;
		}
	}

	readconf();
	gtk_init(&argc, &argv);

	create_window();
}
