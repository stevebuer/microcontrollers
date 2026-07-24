/* entry point */

void system_init(void);
void app_init(void);
void app_loop(void);

void main()
{
	system_init();

	app_init();

	while (1)
		app_loop();
}
