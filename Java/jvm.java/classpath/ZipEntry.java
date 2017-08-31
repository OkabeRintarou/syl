package classpath;

import java.io.*;
import java.util.zip.ZipFile;
import java.util.zip.ZipInputStream;

public class ZipEntry implements Entry {
    private String absPath;

    public ZipEntry(String path) {
        absPath = path;
    }

    @Override
    public byte[] readClass(String className) {
        try {
            ZipFile zf = new ZipFile(absPath);
            InputStream in = new BufferedInputStream(new FileInputStream(absPath));
            ZipInputStream zin = new ZipInputStream(in);
            try {
                java.util.zip.ZipEntry entry;
                while((entry = zin.getNextEntry()) != null) {
                    if(!entry.isDirectory() && entry.getName().equals(className)) {
                        long size = entry.getSize();
                        char[] content = new char[(int)size];
                        BufferedReader reader = new BufferedReader(
                                new InputStreamReader(zf.getInputStream(entry)));
                        reader.read(content);
                        zin.closeEntry();;
                        return new String(content).getBytes();
                    }
                }
            } finally {
                zin.closeEntry();
            }
        } catch(IOException e) {
            return null;
        }

        return null;
    }

    @Override
    public String toString() {
        return absPath;
    }
}