

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
    vector<Command*> menu;

	// you can add data members
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
    void display_menu();
    void init_menu(Data *data);
    void clear_menu();
    };

#endif /* CLI_H_ */
