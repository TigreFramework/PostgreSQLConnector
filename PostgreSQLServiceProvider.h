//
// Created by pedrosoares on 5/21/18.
//

#ifndef TIGREFRAMEWORK_POSTGRESQLSERVICEPROVIDER_H
#define TIGREFRAMEWORK_POSTGRESQLSERVICEPROVIDER_H


#include <TigreFramework/Core/Kernel/Application/Configurable.h>
#include <TigreFramework/Core/Kernel/Application/ServiceProvider.h>

class PostgreSQLServiceProvider : public ServiceProvider, public Configurable {

    public:
        void boot() override ;

};


#endif //TIGREFRAMEWORK_POSTGRESQLSERVICEPROVIDER_H
