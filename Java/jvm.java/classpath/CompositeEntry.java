package classpath;

import java.util.ArrayList;
import java.util.List;

public class CompositeEntry implements Entry  {
    private List<Entry> entries = new ArrayList<>();

    public CompositeEntry() {

    }

    public CompositeEntry(String pathList) {
        String[] paths = pathList.split("/");
        for(String path : paths) {
            entries.add(Entrys.newEntry(path));
        }
    }

    void addEntry(Entry entry) {
        entries.add(entry);
    }

    public byte[] readClass(String className) {
        for(Entry entry : entries) {
            byte[] data = entry.readClass(className);
            if(data != null) {
                return data;
            }
        }
        return null;
    }
}
