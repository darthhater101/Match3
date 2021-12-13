#include "gamefieldmodel_plugin.h"

#include "gamefieldmodel.h"

#include <qqml.h>

void GameFieldModelPlugin::registerTypes(const char *uri)
{
    // @uri GameFieldModel
    qmlRegisterType<GameFieldModel>(uri, 1, 0, "GameFieldModel");
}

