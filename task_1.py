"""
To Do List Application - Python
Based on Codezilla Academy Tutorial
Author: Islam Hesham (Codezilla)
"""

import json
import os

# ═══════════════════════════════════════════════════
# ملف حفظ المهام
# ═══════════════════════════════════════════════════
TASKS_FILE = "tasks.json"


# ═══════════════════════════════════════════════════
# دوال إدارة الملفات (File I/O)
# ═══════════════════════════════════════════════════
def load_tasks():
    """📂 تحميل المهام من الملف"""
    if os.path.exists(TASKS_FILE):
        with open(TASKS_FILE, "r", encoding="utf-8") as file:
            return json.load(file)
    return []


def save_tasks(tasks):
    """💾 حفظ المهام في الملف"""
    with open(TASKS_FILE, "w", encoding="utf-8") as file:
        json.dump(tasks, file, ensure_ascii=False, indent=4)


# ═══════════════════════════════════════════════════
# دوال عرض القائمة والمهام
# ═══════════════════════════════════════════════════
def display_menu():
    """📋 عرض القائمة الرئيسية"""
    print("\n" + "=" * 40)
    print("      📋 TO DO LIST APP")
    print("=" * 40)
    print("1. ➕ إضافة مهمة جديدة")
    print("2. 📋 عرض جميع المهام")
    print("3. ✅ تحديد مهمة كمنجزة")
    print("4. ❌ حذف مهمة")
    print("5. 🚪 خروج")
    print("=" * 40)


def display_tasks(tasks):
    """📄 عرض المهام"""
    if not tasks:
        print("\n📭 لا توجد مهام حالياً!")
        return
    
    print("\n" + "-" * 40)
    print("📋 قائمة المهام:")
    print("-" * 40)
    
    for index, task in enumerate(tasks, start=1):
        status = "✅" if task["done"] else "⬜"
        print(f"{index}. {status} {task['title']}")
    
    print("-" * 40)


# ═══════════════════════════════════════════════════
# دوال العمليات على المهام
# ═══════════════════════════════════════════════════
def add_task(tasks):
    """➕ إضافة مهمة جديدة"""
    title = input("\n📝 أدخل عنوان المهمة: ").strip()
    
    if not title:
        print("❌ عنوان المهمة لا يمكن أن يكون فارغاً!")
        return
    
    task = {
        "title": title,
        "done": False
    }
    tasks.append(task)
    save_tasks(tasks)
    print(f"✅ تمت إضافة المهمة: '{title}'")


def mark_done(tasks):
    """✅ تحديد مهمة كمنجزة"""
    if not tasks:
        print("\n📭 لا توجد مهام!")
        return
    
    display_tasks(tasks)
    
    try:
        choice = int(input("\n🔢 أدخل رقم المهمة: "))
        if 1 <= choice <= len(tasks):
            tasks[choice - 1]["done"] = True
            save_tasks(tasks)
            print("✅ تم تحديد المهمة كمنجزة!")
        else:
            print("❌ رقم غير صالح!")
    except ValueError:
        print("❌ الرجاء إدخال رقم صحيح!")


def delete_task(tasks):
    """❌ حذف مهمة"""
    if not tasks:
        print("\n📭 لا توجد مهام!")
        return
    
    display_tasks(tasks)
    
    try:
        choice = int(input("\n🔢 أدخل رقم المهمة للحذف: "))
        if 1 <= choice <= len(tasks):
            removed = tasks.pop(choice - 1)
            save_tasks(tasks)
            print(f"🗑️ تم حذف المهمة: '{removed['title']}'")
        else:
            print("❌ رقم غير صالح!")
    except ValueError:
        print("❌ الرجاء إدخال رقم صحيح!")
5


# ═══════════════════════════════════════════════════
# الدالة الرئيسية (Game Loop)
# ═══════════════════════════════════════════════════
def main():
    """▶️ التشغيل الرئيسي للتطبيق"""
    tasks = load_tasks()
    
    while True:
        display_menu()
        choice = input("اختر عملية (1-5): ").strip()
        
        if choice == "1":
            add_task(tasks)
        elif choice == "2":
            display_tasks(tasks)
        elif choice == "3":
            mark_done(tasks)
        elif choice == "4":
            delete_task(tasks)
        elif choice == "5":
            print("\n👋 شكراً لاستخدام التطبيق! إلى اللقاء!")
            break
        else:
            print("❌ اختيار غير صالح! حاول مرة أخرى.")


# ═══════════════════════════════════════════════════
# نقطة البداية
# ═══════════════════════════════════════════════════
if __name__ == "__main__":
    main()
    #bnbnbn
    