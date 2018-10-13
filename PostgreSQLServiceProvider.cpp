//
// Created by pedrosoares on 5/21/18.
//

#include <TigreFramework/Core/Kernel/Application/Configuration.h>
#include "PostgreSQLServiceProvider.h"
#include "PostgreSQL.h"

void PostgreSQLServiceProvider::boot() {
    Configuration::set("Database.driver.psql", [](){
        return PostgreSQL::connection();
    });
}