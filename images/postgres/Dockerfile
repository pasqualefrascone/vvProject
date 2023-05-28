#docker run -d --network myNet -e POSTGRES_PASSWORD=password --name pdb pdb


FROM postgres
WORKDIR /docker-entrypoint-initdb.d
#RUN mkdir -p /docker-entrypoint-initdb.d

#ADD ./postgres/init.sql ./docker-entrypoint-initdb.d
COPY ./images/postgres/init.sql .

ENTRYPOINT ["docker-entrypoint.sh"]
EXPOSE 5432
CMD ["postgres"]

