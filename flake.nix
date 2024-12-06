{
  description = "A little library to help with the print debugging on C++11 and forward";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
  let
    headerSrc = builtins.readFile ./CMakeLists.txt;

    versionNumbersRegex = ".*project[[:space:]]*\\(.*[[:space:]]VERSION[[:space:]]+([[:digit:]]+\.[[:digit:]]+\.[[:digit:]]+).*\\).*";

    versionNumbers = builtins.head (builtins.match versionNumbersRegex headerSrc);

    devHash_ = builtins.match ".*DEV_HASH: ([[:xdigit:]]{8}).*" headerSrc;
    devHash = if devHash_ != null then builtins.head devHash_ else null;

    version =
      versionNumbers +
      (
        if devHash != null
        then "+dev." + devHash
        else "+local_repo"
      );

    application = { pkgs }:
      pkgs.stdenv.mkDerivation {
        pname = "icecream-cpp";
        inherit version;

        src = ./.;

        nativeBuildInputs = [ pkgs.buildPackages.cmake ];

        meta = with pkgs.lib; {
          homepage = "https://github.com/renatoGarcia/icecream-cpp";
          description = "A little library to help with the print debugging on C++11 and forward";
          license = licenses.mit;
          maintainers = with maintainers; [ renatoGarcia ];
        };
      };

      dev-env = { pkgs, stdenv }:
        let
          app = application { inherit pkgs; };
        in
          pkgs.mkShell.override { inherit stdenv; } {
            name = "icecream-cpp";
            nativeBuildInputs = with pkgs.buildPackages; [
              (fmt.override { inherit stdenv; })
              boost
              catch2
              clang-tools
              cmakeCurses
              conan
              range-v3
            ] ++ app.nativeBuildInputs;
       };

    in {
      overlays.default = final: prev: {
        icecream-cpp = application { pkgs = final; };
      };
    }
    //
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          overlays = [ self.overlays.default ];
        };
      in
      {
        packages = {
          default = pkgs.icecream-cpp;
        };
        devShells = rec {
          gcc = dev-env { inherit pkgs; stdenv = pkgs.stdenv; };
          clang = dev-env { inherit pkgs; stdenv = pkgs.libcxxStdenv; };
          default = gcc;
        };
      }
    );
}
