/* entry point */

void main()
{
	system_init();

	app_init();

	while (1)
		app_loop();
}
