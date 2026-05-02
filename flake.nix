{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
  };

  outputs = { self, nixpkgs }:
    let pkgs = nixpkgs.legacyPackages.x86_64-linux;
    in {
      devShell.x86_64-linux = pkgs.mkShell {
        buildInputs = with pkgs; [
          clang
          cmake
          sdl3
          sdl3-image
        ];
      };
    };
}
