/****************************************************************************
**
** Copyright (C) 2005 Edmund Bayerle. All rights reserved.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "ImageSyncApp.h"
#include <QtDebug>

int main(int argc, char *argv[])
{
    ImageSyncApp app(argc, argv);
    int execCode = app.exec();
    return execCode;
}

