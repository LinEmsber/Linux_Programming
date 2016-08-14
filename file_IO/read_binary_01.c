

size_t nr;
char buf[64];

nr = fread(buf, sizeof(buf), 1, stream);
if (nr == 0){
        perror("fread");
}
