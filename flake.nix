{
  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable";
  };

  outputs = { nixpkgs, ... }:
    let
      system = "x86_64-linux";
      platform = "mingwW64";
      #pkgs = nixpkgs.legacyPackages.${system};
      pkgs = nixpkgs.legacyPackages.${system}.pkgsCross.${platform};
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        shellHook = ''
          cat >.clangd <<EOF
          CompileFlags:
            Compiler: ${pkgs.buildPackages.gcc.cc}/bin/x86_64-w64-mingw32-gcc
            Add: [-I${pkgs.buildPackages.gcc.cc}/x86_64-w64-mingw32/sys-include, --target=x86_64-w64-mingw32]
          EOF
        '';
      };
    };
}
