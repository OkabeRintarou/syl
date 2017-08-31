package classpath;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;


public class DirEntry implements Entry {
    private String absDir;

    public DirEntry(String path) {
        absDir = path;
    }

    @Override
    public byte[] readClass(String className) {
        File file = new File(absDir + File.pathSeparator + className);
        byte[] content = new byte[(int)file.length()];
        try {
            FileInputStream fin = new FileInputStream(file);
            try {
                fin.read(content);
            } finally {
                fin.close();
            }
        } catch(IOException e) {
            return null;
        }

        return content;
    }

    @Override
    public String toString() {
        return absDir;
    }
}