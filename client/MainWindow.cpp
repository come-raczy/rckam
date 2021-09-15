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

#include "client/MainWindow.hpp"

#include <gtkmm/menuitem.h>
#include <gtkmm/togglebutton.h>

#include "common/Debug.hpp"
#include "common/Exceptions.hpp"

namespace rckam
{
namespace client
{

MainWindow::MainWindow(GtkApplicationWindow *baseObject, const Glib::RefPtr<Gtk::Builder> &builder)
: Gtk::ApplicationWindow(baseObject)
, imagePreview_(nullptr)
, statusBar_(nullptr)
{
  Gtk::MenuItem *quitMenuItem = nullptr;
  builder->get_widget("quitMenuItem", quitMenuItem);
  if (!quitMenuItem)
  {
    RCKAM_THREAD_CERR << "ERROR: failed to find quitMenuItem" << std::endl;
    BOOST_THROW_EXCEPTION(rckam::common::GtkmmException(ENOENT, "failed to find quitMenuItem"));
  }
  quitMenuItem->signal_activate().connect(sigc::mem_fun(*this, &rckam::client::MainWindow::on_quitMenuItem_activate));

  builder->get_widget_derived("imagePreview", imagePreview_);
  if (!imagePreview_)
  {
    RCKAM_THREAD_CERR << "ERROR: failed to find imagePreview" << std::endl;
    BOOST_THROW_EXCEPTION(rckam::common::GtkmmException(ENOENT, "failed to find imagePreview"));
  }

  builder->get_widget("statusBar", statusBar_);
  if (!statusBar_)
  {
    RCKAM_THREAD_CERR << "ERROR: failed to find statusBar" << std::endl;
    BOOST_THROW_EXCEPTION(rckam::common::GtkmmException(ENOENT, "failed to find statusBar"));
  }

  Gtk::ToggleButton *startStopPreviewToggleButton = nullptr;
  builder->get_widget("startStopPreviewToggleButton", startStopPreviewToggleButton);
  if (!startStopPreviewToggleButton)
  {
    RCKAM_THREAD_CERR << "ERROR: failed to find startStopPreviewToggleButton" << std::endl;
    BOOST_THROW_EXCEPTION(rckam::common::GtkmmException(ENOENT, "failed to find startStopPreviewToggleButton"));
  }
  startStopPreviewToggleButton->set_image_from_icon_name("play", START_STOP_ICON_SIZE);
  startStopPreviewToggleButton->signal_toggled().connect(sigc::bind<Gtk::ToggleButton*>(sigc::mem_fun(*this, &rckam::client::MainWindow::on_startStopPreviewToggleButton_toggle), startStopPreviewToggleButton));
  
  
}

MainWindow::~MainWindow()
{
  if (imagePreview_) delete imagePreview_;
  imageLoader_.stop();
}

void MainWindow::setServer(const std::string &ipAddress, const unsigned dataPort)
{
  ipAddress_ = ipAddress;
  dataPort_ = dataPort;
}

void MainWindow::on_startStopPreviewToggleButton_toggle(Gtk::ToggleButton* toggleButton)
{
  toggleButton->set_image_from_icon_name(toggleButton->get_active() ? "pause" : "play", START_STOP_ICON_SIZE);
  RCKAM_THREAD_CERR << "on_startStopPreviewToggleButton_toggle: " << toggleButton->get_active() << std::endl;
  if (toggleButton->get_active())
  {
    if (imageLoader_.isRunning())
    {
      statusBar_->push("Image loader already running");
      return;
    }
    statusBar_->push("Starting image loader");
    assert(nullptr != imagePreview_);
    imageLoader_.start(*imagePreview_, ipAddress_, dataPort_);
  }
  else
  {
    if (!imageLoader_.isRunning())
    {
      statusBar_->push("Image loader is not running");
      return;
    }
    statusBar_->push("Stopping image loader");
    imageLoader_.stop();
  }
}

void MainWindow::open_file_view(const Glib::RefPtr<Gio::File>& /* file */)
{
}

} // namespace client
} // namespace rckam

