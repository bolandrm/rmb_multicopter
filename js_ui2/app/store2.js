import { observable, autorun } from 'mobx'

class TestStore {
  @observable tester = 'yes';

  constructor() {
    this.ok = true
    this.tester;
  }
}

export const testStore = new TestStore()
