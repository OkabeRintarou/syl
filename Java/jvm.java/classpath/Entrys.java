package classpath;

import java.io.File;

public class Entrys {

    public static Entry newEntry(String path) {
        if(path.matches(File.pathSeparator)) {
            return newComposiEntry(path);
        }
        if(path.endsWith("*")) {
            return newWildcardEntry(path);
        }

        String lowerPath = path.toLowerCase();
        if(lowerPath.endsWith(".zip") || lowerPath.endsWith(".jar")) {
            return newZipEntry(path);
        }
        return newDirEntry(path);
    }

    static Entry newComposiEntry(String path) {
        return new CompositeEntry(path);
    }

    static Entry newWildcardEntry(String path) {
        String baseDir = path.substring(0,path.length() - 1);
        CompositeEntry compositeEntry = new CompositeEntry();
        File file = new File(baseDir);
        if(file.exists()) {
            File[] subFiles = file.listFiles();
            for(File subFile : subFiles) {
                if(subFile.isFile() && subFile.getName().toLowerCase().endsWith(".jar")) {
                    compositeEntry.addEntry(newZipEntry(subFile.getAbsolutePath()));
                }
            }
            return compositeEntry;
        }

        return null;
    }

    static Entry newZipEntry(String path) {
        return new ZipEntry(path);
    }

    static Entry newDirEntry(String path) {
        return new DirEntry(path);
    }
}