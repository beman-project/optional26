ARG BASE="steve-downey/cxx-dev:latest"
FROM ${BASE}

USER root
RUN mkdir -p /home/luser/src/cmake.bld \
 && chown -R luser:luser /home/luser/src/cmake.bld

USER luser

RUN touch /tmp/.in_docker
