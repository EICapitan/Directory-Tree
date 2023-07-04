# AT PROJECT Limited 2023
# by ElCapitan
pkgname=ds
_gitname="Directory-diSplay"
pkgver=v1.2.7
pkgrel=1
arch=('x86_64')
license=('GPL3')
source=("git+https://github.com/at-elcapitan/Directory-diSplay.git")
sha256sums=('SKIP')

build() {
    cd "$_gitname"
    make
}

package() {
    cd $_gitname
    mkdir -p $pkgdir/usr/local/bin
    mkdir -p $pkgdir/usr/share/man/man1

    cp build/$pkgname $pkgdir/usr/local/bin
    cp src/ds.1 $pkgdir/usr/share/man/man1/ds.1
}