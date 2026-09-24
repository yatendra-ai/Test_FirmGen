#ifndef APP_H
#define APP_H

/* Vendor-agnostic application entry point. Called once from the vendor reset hook. */
void app_start(void);
void app_process(void);

#endif /* APP_H */
