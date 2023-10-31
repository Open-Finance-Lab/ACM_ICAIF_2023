#build stage
FROM python:3.10-slim as build


RUN apt update \
    && apt install -y --no-install-recommends build-essential libssl-dev cmake git libopenmpi-dev python3-dev zlib1g-dev libgl1-mesa-glx swig \
    && pip install conan numpy pandas polars scipy scikit-learn matplotlib swig box2d

RUN pip install git+https://github.com/AI4Finance-Foundation/FinRL.git "scikit-learn==1.3.0" "numpy==1.26.0" "scipy==1.11.1" "cvxpy==1.3.0"

WORKDIR /app
COPY ./conanfile.py /app/
COPY ./.github/scripts/conan-profile.sh /app/
# hack to make cmake_git_version_tracking not fail the build
RUN git init
RUN git config user.email "hello"
RUN git config user.name "name"
RUN git commit --allow-empty -m "test"

RUN cat conan-profile.sh | bash \
    && conan install . -b missing

COPY . /app
RUN cmake --preset=ci-docker \
    && cmake --build build --config Release -j


# Main stage
FROM python:3.10-slim

# commented out for now just to see if i can even get it woring in the build stage
#RUN pip install numpy pandas polars scipy scikit-learn matplotlib
#
#RUN apt update && apt install -y git build-essential cmake
#RUN pip install swig box2d
#RUN pip install git+https://github.com/AI4Finance-Foundation/FinRL.git "scikit-learn==1.3.0" "numpy==1.26.0" "scipy==1.11.1" "cvxpy==1.3.0"
#RUN pip install conan numpy pandas polars scipy scikit-learn matplotlib swig box2d git+https://github.com/AI4Finance-Foundation/FinRL.git
#RUN apt update \
#    && apt install -y --no-install-recommends build-essential libssl-dev cmake git \
#    && pip install numpy pandas polars scipy scikit-learn matplotlib swig box2d git+https://github.com/AI4Finance-Foundation/FinRL.git

COPY --from=build /app/build/NUTC-client /bin/NUTC-linter
RUN chmod +x /bin/NUTC-linter

CMD NUTC-linter
