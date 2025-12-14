import sys
import hashlib

def file_hash(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        h.update(f.read())
    return h.hexdigest()

def main():
    if len(sys.argv) != 3:
        print("Uso: python comparar.py archivo1 archivo2")
        return

    f1, f2 = sys.argv[1], sys.argv[2]

    h1 = file_hash(f1)
    h2 = file_hash(f2)

    if h1 == h2:
        print("✓ Los archivos son IDENTICOS")
    else:
        print("✗ Los archivos son DIFERENTES")

if __name__ == "__main__":
    main()
