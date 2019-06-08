/**
 **
 ** rckam: a Qt remote control for digital cameras
 **
 ** Copyright (C) <year>  <name of author>
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Affero General Public License as
 ** published by the Free Software Foundation, either version 3 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Affero General Public License for more details.
 **
 ** You should have received a copy of the GNU Affero General Public License
 ** along with this program.  If not, see <https://www.gnu.org/licenses/>.
 **
 **/

  //for (std::vector<std::string>::const_iterator name = getRegistryNameList().begin();
  //     getRegistryNameList().end() != name; ++name)
  //{
  //  CppUnit::Test *namedSuite = CppUnit::TestFactoryRegistry::getRegistry(*name).makeTest();
  //  runner.addTest(namedSuite);
  //}

  // Add the top level (unnamed) suite from the list of tests to run
  CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
  runner.addTest( suite );

  // Change the default outputter to a compiler error format outputter
  runner.setOutputter( new CppUnit::CompilerOutputter( &runner.result(),
                                                       std::cerr ) );
  // Run the tests.
  bool wasSucessful = runner.run();

  // Return error code 1 if the one of test failed.
  return wasSucessful ? 0 : 1;
}
