#!/bin/sh -e


# Installing the script
# Fill in DAEMON with the correct path
#
# sudo cp 430MHz-Deamon.sh /etc/init.d
# sudo chmod +x /etc/init.d/430MHz-Deamon.sh
# sudo chmod +x 430MHz-Deamon.py

DAEMON="/home/beagle/430MHz-Deamon/430MHz-Deamon.py"
DEAMON_NAME="430MHz-Deamon"

PATH="/sbin:/bin:/usr/sbin:/usr/bin"

test -x $DAEMON || exit 0

. /lib/lsb/init-functions

d_start () {
  log_daemon_msg "Starting system $DEAMON_NAME Daemon"
  start-stop-daemon --background --name $DEAMON_NAME --start --exec $DAEMON
  log_end_msg $?
}

d_stop () {
  log_daemon_msg "Stopping system $DEAMON_NAME Daemon"
  start-stop-daemon --name $DEAMON_NAME --stop --retry 5 --name $DEAMON_NAME
  log_end_msg $?
}

case "$1" in

  start|stop)
    d_${1}
  ;;

  restart|reload|force-reload)
    d_stop
    d_start
  ;;

  force-stop)
    d_stop
    killall -q $DEAMON_NAME || true
    sleep 2
    killall -q -9 $DEAMON_NAME || true
  ;;

  status)
    status_of_proc "$DEAMON_NAME" "$DAEMON" "system-wide $DEAMON_NAME" && exit 0 || exit $?
  ;;

  *)
    echo "Usage: /etc/init.d/$DEAMON_NAME {start|stop|force-stop|restart|reload|force-reload|status}"
    exit 1
  ;;

esac
exit 0
