#compile the C++ to WebAssembly
FROM emscripten/emsdk AS wasm-builder

WORKDIR /explicanda

#copy C++ source and header files into their respective directories
COPY ./src ./src
COPY ./include ./include

#create a build folder and compile the binary to Wasm
RUN mkdir -p build && cd build && \
    emcc ../src/*.cpp -I../include -O3 -s ALLOW_MEMORY_GROWTH=1 -sASSERTIONS -s EXIT_RUNTIME=0 -s MODULARIZE=1 -s EXPORT_ES6=1 -s INVOKE_RUN=0 -s "EXPORTED_FUNCTIONS=['_main','_fflush']" -s "EXPORTED_RUNTIME_METHODS=['callMain','ccall','cwrap']" -o evaluate.js


#build the Astro SSR server
FROM node:22-slim

WORKDIR /explicanda/portal/

#copy package properties to install Node modules
COPY portal/package*.json ./
RUN npm install

#copy all Astro project source files into the container
COPY portal/ ./

#copy the generated Wasm files into the exact spots vite and emscripten expect them
#js goes to src/ so vite can bundle it
COPY --from=wasm-builder /explicanda/build/evaluate.js ./src/wasm/evaluate.js
#wasm goes to public/ so it can be fetched as a static asset
COPY --from=wasm-builder /explicanda/build/evaluate.wasm ./public/wasm/evaluate.wasm

#run the Astro build command
RUN npm run build

#astro's node server defaults to port 4321
EXPOSE 4321

#set environment variables so Astro listens on all network interfaces inside Docker
ENV HOST=0.0.0.0
ENV PORT=4321

#command to launch the compiled Astro server
CMD ["node", "./dist/server/entry.mjs"]