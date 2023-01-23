for file in ./Toronto/*
do 
    filename="${file%.*}"
    echo $filename ;
done
