#include <TigreFramework/Core/Kernel/Application/Configuration.h>
#include "PostgreSQLServiceProvider.h"
#include "PostgreSQLConfiguration.h"

void PostgreSQLServiceProvider::boot() {
    Configuration::set("Database.driver.pgsql", new PostgreSQLConfiguration());
    //Configuration::set("Database.driver.", new PostgreSQL());
}