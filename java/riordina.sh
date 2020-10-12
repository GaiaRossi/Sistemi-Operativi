echo "Riordino i tuoi brutti file."

dirclass="class"

if test ! -d "$dirclass"
then
    mkdir $dirclass
fi

mv *.class $dirclass/

echo "Ho spostato i file class."