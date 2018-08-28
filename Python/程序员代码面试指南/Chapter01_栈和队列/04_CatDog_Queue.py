#!/usr/bin/python3
# -*- coding:utf-8 -*-
from collections import deque


class Pet(object):
    def __init__(self, t):
        self.type = t

    def __repr__(self):
        return self.type


class Cat(Pet):
    def __init__(self):
        super().__init__('cat')


class Dog(Pet):
    def __init__(self):
        super().__init__('dog')


class PetEntry(object):
    count = 0

    def __init__(self, pet):
        self.pet = pet
        self.count = PetEntry.count
        PetEntry.count += 1

    def get_pet(self):
        return self.pet

    def timestamp(self):
        return self.count


class CatDogQueue(object):
    def __init__(self):
        self.cats = deque()
        self.dogs = deque()

    def add(self, pet):
        if isinstance(pet, Dog):
            self.dogs.append(PetEntry(pet))
        elif isinstance(pet, Cat):
            self.cats.append(PetEntry(pet))
        else:
            raise ValueError('queue only store cat or dog instance')

    def poll_all(self):
        res = []
        while len(self.cats) > 0 and len(self.dogs) > 0:
            if self.dogs[0].timestamp() < self.cats[0].timestamp():
                res.append(self.dogs.popleft().get_pet())
            else:
                res.append(self.cats.popleft().get_pet())
        if len(self.cats) > 0:
            size = len(self.cats)
            for i in range(size):
                res.append(self.cats.popleft().get_pet())
        if len(self.dogs) > 0:
            size = len(self.dogs)
            for i in range(size):
                res.append(self.dogs.popleft().get_pet())
        return res

    def poll_dog(self):
        dogs = []
        while len(self.dogs) > 0:
            dogs.append(self.dogs.popleft().get_pet())
        return dogs

    def poll_cat(self):
        cats = []
        while len(self.cats) > 0:
            cats.append(self.cats.popleft().get_pet())
        return cats

    def is_empty(self):
        return len(self.cats) == 0 and len(self.dogs) == 0

    def is_dog_empty(self):
        return len(self.dogs) == 0

    def is_cat_empty(self):
        return len(self.cats) == 0


if __name__ == '__main__':
    def build():
        q = CatDogQueue()
        q.add(Cat())
        q.add(Dog())
        q.add(Cat())
        return q


    def test_poll_all():
        q = build()
        print(q.poll_all())


    def test_poll():
        q = build()
        print(q.poll_cat())
        print(q.is_cat_empty())
        print(q.is_dog_empty())
        print(q.poll_dog())
        print(q.is_dog_empty())


    test_poll_all()
    test_poll()
