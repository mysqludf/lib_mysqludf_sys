FROM mariadb

RUN apt-get update && \
	apt-get install -y \
        gcc \
        libmysqlclient-dev \
        nano \
        && rm -rf /var/lib/apt/lists/*

WORKDIR /root/lib_mysqludf_sys

COPY lib_mysqludf_sys.c ./
COPY lib_mysqludf_sys.sql /docker-entrypoint-initdb.d/.
COPY hello_world.sh /home/mysql/hello_world.sh

RUN gcc -Wall -I/usr/include/mysql -I. -shared lib_mysqludf_sys.c -o $(mysql_config --plugindir)/lib_mysqludf_sys.so
