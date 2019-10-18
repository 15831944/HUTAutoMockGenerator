#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <clang-c/Index.h>
#include <set>
#include <chrono>
#include <fstream>

#include "Utils.h"
#include "MockClassGenerator.h"
#include "MockMethodGenerator.h"
#include "Constants.h"
#include "CommonData.h"

CXChildVisitResult classVisitor(CXCursor c, CXCursor p, CXClientData ccd);
CXChildVisitResult method_visitor(CXCursor c, CXCursor p, CXClientData cd);
