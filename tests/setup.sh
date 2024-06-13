str="#define NOSYSTEM system"

for file in $(find . -name "*.c"); do
  echo Processing $file
  printf "$str\n" > $file.modified
  cat $file >>  $file.modified 

  mv $file.modified $file
  rm -f $file.modified
done