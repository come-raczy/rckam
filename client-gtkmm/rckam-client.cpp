/**
 ** rckam
 ** Copyright (c) 2020-2021 Come Raczy
 ** All rights reserved.
 **
 ** This software is provided under the terms and conditions of the
 ** GNU AFFERO GENERAL PUBLIC LICENSE
 **
 ** You should have received a copy of the
 ** GNU AFFERO GENERAL PUBLIC LICENSE
 ** along with this program. If not, see
 ** <https://fsf.org/>
 **/

#include "common/Debug.hpp"
#include "common/Exceptions.hpp"
#include "client-gtkmm/RckamOptions.hpp"

void rckamGui(const rckam::client::RckamOptions &options);

int main(int argc, char *argv[])
{
  rckam::common::run(rckamGui, argc, argv);
}

void rckamGui(const rckam::client::RckamOptions &options)
{
  RCKAM_THREAD_CERR << "INFO: rckamGui: " << options.description << std::endl;
}

