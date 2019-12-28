#pragma once

// @TODO: license header

#include "WriteStreamCSV.h"

namespace souffle {

class WriteStreamNew : public WriteStreamCSV {
    // @TODO: this class as wrapper for read stream csv
};

class WriteNewFactory : public WriteFileCSVFactory {
    // @TODO: this class as wrapper for csv file factory
};

}  // namespace souffle
