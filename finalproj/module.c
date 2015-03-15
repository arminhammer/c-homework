/*
 * module.c
 *
 *  Created on: Apr 20, 2014
 *      Author: armin
 */

#include "module.h"

/*
 * Loads a dynamic module
 */
loadModule(char *plugin_name, char file_name[80], void *plugin) {

  plugin = dlopen (file_name, RTLD_NOW);
  if (!plugin)
  {
     fatal ("Cannot load %s: %s", plugin_name, dlerror ());
  }

}
