FROM reg.lily.kazv.moe/kazv/libkazv-deps/deps:servant

RUN mkdir -pv /build
COPY . /build/libkazv

WORKDIR /build/libkazv

ARG BUILD_TYPE=Debug
ARG JOBS=3
RUN mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE -Dlibkazv_BUILD_TESTS=ON -Dlibkazv_BUILD_EXAMPLES=ON -Dlibkazv_BUILD_KAZVJOB=ON && \
    make -j$JOBS && \
    ./src/tests/kazvtest && \
    make -j$JOBS install && \
    cd ../.. && \
    rm -rf /build/libkazv
