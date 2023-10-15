{
  description = "Volumetric processing toolkit and C++ libraries for the recovery and restoration of damaged cultural materials";

  inputs = {
    nixpkgs = {
      url = "github:nixos/nixpkgs/nixpkgs-unstable";
    };
    acvd-src = {
      url = "gitlab:educelab/acvd/9efaeb4d";
      flake = false;
    };
    bvh-src = {
      url = "github:madmann91/bvh/19dde37";
      flake = false;
    };
    googletest-src = {
      url = "github:google/googletest/389cb68";
      flake = false;
    };
    indicators-src = {
      url = "github:p-ranav/indicators/4ea7161";
      flake = false;
    };
    json-src = {
      url = "github:nlohmann/json/v3.11.2";
      flake = false;
    };
    openabf-src = {
      url = "gitlab:educelab/OpenABF/63afa9e3";
      flake = false;
    };
    smgl-src = {
      url = "gitlab:educelab/smgl/61425333";
      flake = false;
    };
  };

  outputs =
    { self
    , nixpkgs
    , acvd-src
    , bvh-src
    , googletest-src
    , indicators-src
    , json-src
    , openabf-src
    , smgl-src
    }:
    let
      version = builtins.substring 0 8 self.lastModifiedDate;
      supportedSystems = [ "x86_64-linux" "x86_64-darwin" "aarch64-linux" "aarch64-darwin" ];
      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;
      nixpkgsFor = forAllSystems (system: import nixpkgs { inherit system; overlays = [ self.overlays.default ]; });
    in
    {

      overlays.default = final: prev: {

        acvd = with final; stdenv.mkDerivation rec {
          pname = "acvd";
          version = acvd.src.shortRev;

          src = acvd-src;

          nativeBuildInputs = [ cmake ];
          buildInputs = [
            vtk
          ];
        };

        volume-cartographer = with final; stdenv.mkDerivation rec {
          pname = "volume-cartographer";
          inherit version;
          src = self;
          nativeBuildInputs = [
            cmake
            qt6.wrapQtAppsHook
          ];
          buildInputs = [
            acvd
            boost
            eigen
            itk
            libtiff
            opencv
            qt6.qtbase
            spdlog
            vtk
          ];
          cmakeFlags =
            [
              "-DVC_BUILD_TESTS=ON"
              "-DFETCHCONTENT_FULLY_DISCONNECTED=ON"
              "-DFETCHCONTENT_QUIET=OFF"
              "-DFETCHCONTENT_TRY_FIND_PACKAGE_MODE=ALWAYS"
              "-DFETCHCONTENT_SOURCE_DIR_BVH=${bvh-src}"
              "-DFETCHCONTENT_SOURCE_DIR_GOOGLETEST=${googletest-src}"
              "-DFETCHCONTENT_SOURCE_DIR_INDICATORS=${indicators-src}"
              "-DFETCHCONTENT_SOURCE_DIR_JSON=${json-src}"
              "-DFETCHCONTENT_SOURCE_DIR_OPENABF=${openabf-src}"
              "-DFETCHCONTENT_SOURCE_DIR_SMGL=${smgl-src}"
            ];
          doCheck = true;
          meta = {
            mainProgram = "VC";
          };
        };

      };

      packages = forAllSystems (system:
        {
          default = nixpkgsFor.${system}.volume-cartographer;
          inherit (nixpkgsFor.${system}) volume-cartographer;
        });

    };
}
