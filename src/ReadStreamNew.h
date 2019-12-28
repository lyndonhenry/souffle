#pragma once

// @TODO: license header

#include "ReadStreamCSV.h"

namespace souffle {

class ReadStreamNew : public ReadStreamCSV {
    // @TODO: this class as wrapper for read stream csv
};

class ReadNewFactory : public ReadFileCSVFactory {
    // @TODO: this class as wrapper for csv file factory
};

}  // namespace souffle
